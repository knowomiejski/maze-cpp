#include <iostream>
#include <vector>
#include <string>
#include <random>

using namespace std;

enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

struct Item {
    int id;
    string itemName;
    string itemDescription;
};

struct Room {
    int id;
    string roomName;
    string roomDescription;
    string conditionalRoomSucceededDescription;
    string conditionalRoomFailedDescription;
    int roomToTheNorth;
    int roomToTheEast;
    int roomToTheSouth;
    int roomToTheWest;
    Item *requiredItem;
    Item *containsItem;
    bool finalRoom;
};

struct Player {
    int currentRoomId;
    Item *itemsInInventory[4];
    string pathTaken;
};

int main()
{

    goto initialize_player; 

initialize_player:
    Player player = {
        0,
        {nullptr, nullptr, nullptr, nullptr},
        ""
    };

    player.pathTaken = "The path followed by the player: \n";

assign_number_of_rooms:
    int roomAmount = 8;
    // https://en.wikipedia.org/wiki/Lehmer_random_number_generator
    // In this case we use the address as a seed for the random number generator
    uint32_t randomNumber = (uint64_t) &randomNumber * 48271 % 0x7fffffff;
    roomAmount = 8 + randomNumber % 10;

    (uint32_t) randomNumber = (uint64_t) roomAmount * 48271 % 0x7fffffff;
    int createCircleAtIndex = randomNumber % (roomAmount - 4);
    
    cout << "Amount of rooms: " << roomAmount << endl;
    cout << "Circle at: " << createCircleAtIndex << endl;

create_maze:
    Room* maze = new Room[roomAmount];

    for (int i = 0; i < roomAmount; i++) {
        maze[i] = {
            i+1, //id
            " room", // roomName
            "", // roomDescription
            "", // conditionalRoomSucceededDescription
            "", // conditionalRoomFailedDescription
            0, // roomToTheNorth
            0, // roomToTheEast
            0, // roomToTheSouth
            0, // roomToTheWest
            nullptr, // requiredItem
            nullptr, // containsItem
            false // finalRoom
        };
    }

    //Create circle
    maze[createCircleAtIndex].roomToTheNorth = maze[createCircleAtIndex + 1].id;
    
    maze[createCircleAtIndex + 1].roomToTheSouth = maze[createCircleAtIndex].id;
    maze[createCircleAtIndex + 1].roomToTheEast = maze[createCircleAtIndex + 2].id;

    maze[createCircleAtIndex + 2].roomToTheWest = maze[createCircleAtIndex + 1].id;
    maze[createCircleAtIndex + 2].roomToTheSouth = maze[createCircleAtIndex + 3].id;
    
    maze[createCircleAtIndex + 3].roomToTheNorth = maze[createCircleAtIndex + 2].id;
    maze[createCircleAtIndex + 3].roomToTheWest = maze[createCircleAtIndex].id;

    maze[createCircleAtIndex].roomToTheEast = maze[createCircleAtIndex + 3].id;

    // used as switch for the saturated/available rooms adding
    bool assigningRooms = false;

    // 2 Arrays used for seeing if a room is available to place an adjecent room into
    int *fullySaturatedRooms = new int[roomAmount];
    for (int i = 0; i < roomAmount; i ++) {
        fullySaturatedRooms[i] = 0;
    }
    int *availableRooms = new int[roomAmount];
    for (int i = 0; i < roomAmount; i ++) {
        availableRooms[i] = 0;
    }
    int currentlyCheckingRoomForSaturation = 0;
    int currentlyCheckingAvailableRoomForSaturation = 0;

check_saturated_rooms:
    if (currentlyCheckingRoomForSaturation >= roomAmount) {
        goto prepare_assign_adjecent_rooms;
    }
    for (int i = currentlyCheckingRoomForSaturation; i < roomAmount; i++) {
        // check for saturation
        if
        (
            maze[i].roomToTheNorth != 0 &&
            maze[i].roomToTheEast != 0 &&
            maze[i].roomToTheSouth != 0 &&
            maze[i].roomToTheWest != 0
        ) {
            goto add_to_saturated_rooms;
        } else {
            goto add_to_available_rooms;
        }
    }
    goto prepare_assign_adjecent_rooms;

add_to_available_rooms:
    bool alreadyAddedToAvailable = false;
    for (int i = 0; i < roomAmount; i++) {
        if (availableRooms[i] == maze[currentlyCheckingRoomForSaturation].id) {
            // already added;
            alreadyAddedToAvailable = true;
            break;
        }
    }
    if (!alreadyAddedToAvailable) {
        for (int i = 0; i < roomAmount; i++) {
            if (availableRooms[i] == 0) {
                availableRooms[i] = maze[currentlyCheckingRoomForSaturation].id;
                break;
            }
        }
    }

    // Remove from the saturated rooms
    for (int i = 0; i < roomAmount; i++) {
        if (fullySaturatedRooms[i] == maze[currentlyCheckingRoomForSaturation].id) {
            fullySaturatedRooms[i] = 0;
            break;
        }
    }

    currentlyCheckingRoomForSaturation = currentlyCheckingRoomForSaturation + 1;
    currentlyCheckingAvailableRoomForSaturation = currentlyCheckingAvailableRoomForSaturation + 1;

    if (!assigningRooms) {
        goto check_saturated_rooms;
    } else {
        goto assign_adjecent_rooms;
    }

add_to_saturated_rooms:
    bool alreadyAddedToSaturated = false;
    for (int i = 0; i < roomAmount; i++) {
        if (fullySaturatedRooms[i] == maze[currentlyCheckingRoomForSaturation].id) {
            // already added;
            alreadyAddedToSaturated = true;
            break;
        }
    }
    if (!alreadyAddedToSaturated) {
        for (int i = 0; i < roomAmount; i++) {
            if (fullySaturatedRooms[i] == 0) {
                fullySaturatedRooms[i] = maze[currentlyCheckingRoomForSaturation].id;
                break;
            }
        }
    }

    // Remove from the available rooms
    for (int i = 0; i < roomAmount; i++) {
        if (availableRooms[i] == maze[currentlyCheckingRoomForSaturation].id) {
            availableRooms[i] = 0;
            break;
        }
    }

    currentlyCheckingRoomForSaturation = currentlyCheckingRoomForSaturation + 1;
    currentlyCheckingAvailableRoomForSaturation = currentlyCheckingAvailableRoomForSaturation + 1;

    if (!assigningRooms) {
        goto check_saturated_rooms;
    } else {
        goto assign_adjecent_rooms;
    }

prepare_assign_adjecent_rooms:
    int sizeOfAvailableRooms = 0;
    for (int i = 0; i < roomAmount; i ++) {
        if (availableRooms[i] != 0) {
            sizeOfAvailableRooms++;
        }
    }

    int sizeOfFullySaturatedRooms = 0;
    for (int i = 0; i < roomAmount; i ++) {
        if (fullySaturatedRooms[i] != 0) {
            sizeOfFullySaturatedRooms++;
        }
    }

    currentlyCheckingRoomForSaturation = 0;
    currentlyCheckingAvailableRoomForSaturation = 0;


assign_adjecent_rooms:
    for (int i = currentlyCheckingAvailableRoomForSaturation; i < sizeOfAvailableRooms; i ++) {
        if (currentlyCheckingAvailableRoomForSaturation >= sizeOfAvailableRooms) {
            goto prepare_add_items_to_rooms;
        }
        // Rooms are added in order 0 = north, 1 = south, etc.
        int *roomsToAvoid = new int[4]; 
        for (int j = 0; j < 4; j++) {
            roomsToAvoid[j] = 0;
        }
        int maxRoomsToAdd = 0;
        for (int j = currentlyCheckingRoomForSaturation; j < roomAmount; j++) {
            if (currentlyCheckingRoomForSaturation >= roomAmount) {
                break;
            }
            if (availableRooms[i] != maze[j].id) continue;
            // cout << availableRooms[i] << " \n" << endl;
            currentlyCheckingRoomForSaturation = j;
            if (maze[j].roomToTheNorth != 0) {
                roomsToAvoid[0] = maze[j].roomToTheNorth;
            }
            
            if (maze[j].roomToTheEast != 0) {
                roomsToAvoid[1] = maze[j].roomToTheEast;
            }
            
            if (maze[j].roomToTheSouth != 0) {
                roomsToAvoid[2] = maze[j].roomToTheSouth;
            }
            
            if (maze[j].roomToTheWest != 0) {
                roomsToAvoid[3] = maze[j].roomToTheWest;
            }

            // Add amount of rooms that can be added
            for (int k = 0; k < 4; k++) {
                if (roomsToAvoid[k] == 0){
                    maxRoomsToAdd++;
                }
            }

            // This means the room is fully saturated
            if (maxRoomsToAdd == 0) {
                goto add_to_saturated_rooms;
            }

            // Choose random number for how many rooms to try to add
            (uint32_t) randomNumber = (uint64_t) randomNumber * 48271 % 0x7fffffff;
            int howManyRoomsToAdd = (randomNumber % maxRoomsToAdd);
            // cout << "RN howManyRoomsToAdd: " << howManyRoomsToAdd << endl;
            if (howManyRoomsToAdd == 0) {
                howManyRoomsToAdd = 1;
            }

            // int howManyRoomsToAdd = 1;
            // cout << "RN howManyRoomsToAdd: " << howManyRoomsToAdd << endl;


            // Every room to add 
            for (int k = 0; k < howManyRoomsToAdd; k++) {
                (uint32_t) randomNumber = (uint64_t) randomNumber * 48271 % 0x7fffffff;
                int directionToAdd = randomNumber % 4;
                // cout << "RN directionToAdd: " << directionToAdd << endl;
                if (roomsToAvoid[directionToAdd] != 0) continue; //direction already taken, not adding room
                
                for (int l = 0; l < roomAmount; l++) {
                    // cout << "Trying add to room: " << availableRooms[l] << " to room: " << maze[j].id << endl;
                    // check if the room can be added
                    if (availableRooms[l] == maze[j].id) {
                        // cout << "Cant add to itself - skip" << endl;
                        continue;
                    }
                    if (availableRooms[l] == 0) continue;
                    bool skipRoom = false;
                    for (int m = 0; m < 4; m++) {
                        if(roomsToAvoid[m] == availableRooms[l]) {
                            skipRoom = true;
                            break;
                        }
                    }

                    //check if the room to be added has the direction available
                    Room *roomToAddPtr = nullptr;
                    for (int m = 0; m < roomAmount; m++) {
                        if (maze[m].id == availableRooms[l]) {
                            roomToAddPtr = &maze[m];
                        }
                    }

                    if (roomToAddPtr == nullptr) continue;
                    int roomToAddAdjecentRooms[4] = { roomToAddPtr->roomToTheNorth, roomToAddPtr->roomToTheEast, roomToAddPtr->roomToTheSouth, roomToAddPtr->roomToTheWest};

                    if (roomToAddAdjecentRooms[0] != 0 && directionToAdd == 2) {
                        skipRoom = true;
                    }
                    
                    if (roomToAddAdjecentRooms[1] != 0 && directionToAdd == 3) {
                        skipRoom = true;
                    }

                    if (roomToAddAdjecentRooms[2] != 0 && directionToAdd == 0) {
                        skipRoom = true;
                    }

                    if (roomToAddAdjecentRooms[3] != 0 && directionToAdd == 1) {
                        skipRoom = true;
                    }

                    // If the relation in the room we want to add matches with an already existing relation in this room (maze[j]),
                    // it means that a triangle will be made - we dont want that
                    // Don't check 0 (empty)
                    // The rooms still can become linked to each other through the edges of the map (like hidden passages)
                    for (int m = 0; m < 4; m++) {
                        if (roomsToAvoid[m] == 0) continue;
                        for (int n = 0; n < 4; n++) {
                            if (roomToAddAdjecentRooms[n] == 0) continue;
                            if (roomsToAvoid[m] == roomToAddAdjecentRooms[n]) {
                                // cout << "Checking: " << roomsToAvoid[m] << " - " << roomToAddAdjecentRooms[n] << endl;
                                skipRoom = true;
                            }
                        }
                    }

                    if (skipRoom) continue;
                    // cout << "Adding: " << roomToAddPtr->id << " - " << roomToAddPtr->roomName << " to " << maze[j].id << " - " << maze[j].roomName << endl;

                    if (directionToAdd == 0) {
                        maze[j].roomToTheNorth = roomToAddPtr->id;
                        roomToAddPtr->roomToTheSouth = maze[j].id;
                        roomsToAvoid[directionToAdd] = roomToAddPtr->id;
                    } else if (directionToAdd == 1) {
                        maze[j].roomToTheEast = roomToAddPtr->id;
                        roomToAddPtr->roomToTheWest = maze[j].id;
                        roomsToAvoid[directionToAdd] = roomToAddPtr->id;
                    } else if (directionToAdd == 2) {
                        maze[j].roomToTheSouth = roomToAddPtr->id;
                        roomToAddPtr->roomToTheNorth = maze[j].id;
                        roomsToAvoid[directionToAdd] = roomToAddPtr->id;
                    } else if (directionToAdd == 3) {
                        maze[j].roomToTheWest = roomToAddPtr->id;
                        roomToAddPtr->roomToTheEast = maze[j].id;
                        roomsToAvoid[directionToAdd] = roomToAddPtr->id;
                    }
                    break;
                }
            }
            // cout << "1 Finished adding to: " << maze[j].id << " - " << maze[j].roomName << endl;

            maxRoomsToAdd = 0;
            // Add amount of rooms that can be added
            for (int k = 0; k < 4; k++) {
                if (roomsToAvoid[k] == 0){
                    maxRoomsToAdd++;
                }
            }

            // This means the room is fully saturated
            if (maxRoomsToAdd == 0) {
                goto add_to_saturated_rooms;
            }
            // cout << "2 Finished adding to: " << maze[j].id << " - " << maze[j].roomName << endl;
        }
        //check which rooms are already in room x
        currentlyCheckingAvailableRoomForSaturation++;
        // cout << "3 Finished adding roomAmount... " << roomAmount << endl;
        // cout << "3 Finished adding sizeOfAvailableRooms... " << sizeOfAvailableRooms << endl;
        // cout << "3 Finished adding currentlyCheckingAvailableRoomForSaturation... " << currentlyCheckingAvailableRoomForSaturation << endl;
    }
    // cout << "4 Finished adding... " << endl;

/*
    bool hasACircle = false;
check_for_circle_of_rooms:
    for (int i = 0; i < roomAmount; i++) {
        // Start looking from bottom-left, in a righthanded circle (top-left, top-right, etc.)
        if (maze[i].roomToTheNorth != 0) {
            int circleWithRooms[4] = {maze[i].id, maze[i].roomToTheNorth, 0, 0};

            Room *roomToCheckPtr = nullptr;
            for (int j = 0; j < roomAmount; j++) {
                if (maze[i].roomToTheNorth == maze[j].id) {
                    // Room - top-left
                    roomToCheckPtr = &maze[j];
                }
            }
            // cout << "TL: "<< roomToCheckPtr->roomName << endl;

            if (roomToCheckPtr->roomToTheEast == 0) continue;
            circleWithRooms[2] = roomToCheckPtr->roomToTheEast;

            for (int j = 0; j < roomAmount; j++) {
                if (roomToCheckPtr->roomToTheEast == maze[j].id) {
                    // Room top-right
                    roomToCheckPtr = &maze[j];
                }
            }
            // cout << "TR: "<< roomToCheckPtr->roomName << endl;

            if (roomToCheckPtr->roomToTheSouth == 0) continue;
            circleWithRooms[3] = roomToCheckPtr->roomToTheSouth;

            for (int j = 0; j < roomAmount; j++) {
                if (roomToCheckPtr->roomToTheSouth == maze[j].id) {
                    // Room bottom-right
                    roomToCheckPtr = &maze[j];
                }
            }
            // cout << "BR: "<< roomToCheckPtr->roomName << endl;

            if (roomToCheckPtr->roomToTheWest == maze[i].id) {
                //Circle complete!
                cout << "Circle between rooms: " << endl;
                cout << to_string(circleWithRooms[0]) << endl;
                cout << to_string(circleWithRooms[1]) << endl;
                cout << to_string(circleWithRooms[2]) << endl;
                cout << to_string(circleWithRooms[3]) << endl;
                hasACircle = true;
                break;
            }
        }
    }
*/
prepare_add_items_to_rooms:
    // Has a torch 
    string firstItemRoomName = "Light";

    string conditionalRoomAdjectives[4] = {
        "Locked ", // Needs key - exit
        "Creepy", // Needs newspaper - Has key
        "Secret", // Needs secret note - Has newspaper
        "Dark", // Needs torch - Has note
    };

    string conditionalRoomFailedDescriptions[4] = {
        "You see a big steel door with a key hole in it. When trying to open it, it doens't budge. It seems to be locked.", // Needs key - exit
        "When entering the room, you see spiderwebs everywhere. Out of the corner of your eye you spot a HUGE spider (at least 2 cm in length). No way you're getting through this room without a serious weapon...", // Needs newspaper - Has key
        "You don't see any door in this direction, however you get a feeling that this wall look a bit suspicious...", // Needs secret note - Has newspaper
        "The room you enter has no lightsource whatsoever - it's pitch-black. After stumbeling through the dark for a bit you feel like there's no way you can navigate this room without a lightsource...", // Needs torch - Has note
    };

    string conditionalRoomSucceededDescriptions[4] = {
        "You walk to the steel door and insert the key. After turning the key the door opens!", // Needs key - exit
        "Armed with a rolled up newspaper, the spiders don't stand a chance. You fight your way through the room!", // Needs newspaper - Has key
        "You look at the wall and see that it matches the drawing you picked up. You find the stone circled on the drawing and push it - the wall swings open revealing a passage.", // Needs secret note - Has newspaper
        "Having the torch in your makes navigating the room much easier. You enter the room.", // Needs torch - Has note
    };

    Item availableItems[4] = {
        { 4, "Key", "It seems to be a heavy-duty key for a heavy-duty door." },
        { 3, "Rolled-up newspaper", "Dated 24-01-2025. Doesn't have anything interesting written in it. A formidable weapon when rolled up." },
        { 2, "Secret note", "A drawing of a wall with a specific stone circled. The note says: 'PRESS' " },
        { 1, "Torch", "A convienient source of light." },
    };

    int addedItems = 0;
    int maxAdjecent = 1;

    int roomsAdjecentToFirst[4] = {maze[0].roomToTheNorth, maze[0].roomToTheEast, maze[0].roomToTheSouth, maze[0].roomToTheWest};
    for (int i = 0; i < 4; i++) {
        if (roomsAdjecentToFirst[i] != 0) {
            // Assign the first item to a room that won't be locked and is next to the starting point
            maze[roomsAdjecentToFirst[i] - 1].containsItem = &availableItems[3];
            maze[roomsAdjecentToFirst[i] - 1].roomName = firstItemRoomName;
            maze[roomsAdjecentToFirst[i] - 1].roomDescription = "The room is illuminated by torches, like a hall in a medieval castle.";
            break;
        }
    }

add_items_to_rooms:
    // Start from the end, since the less populated rooms are at the end of the maze array
    // Don't add item or requirement to the first room
    for (int i = roomAmount - 1; i > 1; i--) {
        // skip already assigned rooms
        if (maze[i].containsItem != nullptr || maze[i].requiredItem != nullptr) continue;
        int adjecentCount = 0;
        if (maze[i].roomToTheNorth != 0) {
            adjecentCount++;
        }

        if (maze[i].roomToTheEast != 0) {
            adjecentCount++;
        }

        if (maze[i].roomToTheSouth != 0) {
            adjecentCount++;
        }

        if (maze[i].roomToTheWest != 0) {
            adjecentCount++;
        }

        if (adjecentCount > maxAdjecent) continue;
        if (addedItems <= 0) {
            maze[i].finalRoom = true;
        } else {
            maze[i].containsItem = &availableItems[addedItems - 1];
        }
        maze[i].roomName = conditionalRoomAdjectives[addedItems];
        maze[i].conditionalRoomSucceededDescription = conditionalRoomSucceededDescriptions[addedItems];
        maze[i].conditionalRoomFailedDescription = conditionalRoomFailedDescriptions[addedItems];
        maze[i].requiredItem = &availableItems[addedItems];
        addedItems++;
        if (addedItems == 4) break;
    }

    // If there are no rooms on the edges anymore, increase the amount of adjecent needed
    if (addedItems < 4) {
        maxAdjecent++;
        goto add_items_to_rooms;
    }

    // assign rest of the room names
    string roomNameAdjectives[18] {
        "Cold",
        "Small",
        "Clean",
        "Huge",
        "Messy",
        "Narrow",
        "Quiet",
        "Stone",
        "Pleasant",
        "Furnished",
        "Bare",
        "Empty",
        "Little",
        "Fancy",
        "Tidy",
        "Vibey",
        "Strange",
        "Wooden",
    };

    for (int i = 0; i < roomAmount; i++) {
        if (maze[i].roomName == " room") {
            maze[i].roomName = roomNameAdjectives[i];
        }
    }

place_player:
    char direction;
    player.currentRoomId = 1;
    Room roomToMoveTo = maze[player.currentRoomId - 1];
    cout << "============================================================================" << endl;
    cout << "You wake up in a strange empty mansion." << endl;
    cout << "You are not sure what this place is, but you should find a way out asap..." << endl;
    cout << "============================================================================" << endl;
    
ask_direction:
    cout << endl;
    cout << "============================================================================" << endl;
    cout << "You are now in a " << maze[player.currentRoomId - 1].roomName  << " room"<< endl;
    cout << "Which way do you want to go? Type N, E, S or W to head in that direction." << endl;
    cout << "============================================================================" << endl;
    scanf(" %c", &direction);

move_to_room:
    int roomToMoveToId = 0;
    string directionAsWord = "";
    if (direction == 'N') {
        roomToMoveToId = maze[player.currentRoomId - 1].roomToTheNorth;
        directionAsWord = "north";
    } else if (direction == 'E') {
        roomToMoveToId = maze[player.currentRoomId - 1].roomToTheEast;
        directionAsWord = "east";
    } else if (direction == 'S') {
        roomToMoveToId = maze[player.currentRoomId - 1].roomToTheSouth;
        directionAsWord = "south";
    } else if (direction == 'W') {
        roomToMoveToId = maze[player.currentRoomId - 1].roomToTheWest;
        directionAsWord = "west";
    } else {
        cout << "You walk around confused... Seems like you have forgotten the directions on a compass." << direction << endl;
        goto ask_direction;
    }
    cout << "You head "<< directionAsWord << "." << endl;

    if (roomToMoveToId > 0) {
        if (maze[roomToMoveToId - 1].requiredItem != nullptr) {
            bool hasRequiredItem = false;
            for (int i = 0; i < 4; i++) {
                if (player.itemsInInventory[i] == maze[roomToMoveToId - 1].requiredItem) {
                    hasRequiredItem = true;
                    break;
                }
            }
            if (hasRequiredItem) {
                cout << maze[roomToMoveToId - 1].conditionalRoomSucceededDescription << endl;
            } else {
                cout << maze[roomToMoveToId - 1].conditionalRoomFailedDescription << endl;
                goto ask_direction;
            }
        }
        
        if (maze[roomToMoveToId- 1].containsItem != nullptr) {
            //
            if (maze[roomToMoveToId - 1].roomDescription != "") {
                cout << maze[roomToMoveToId - 1].roomDescription << endl;
            }
            cout << "You found: " << maze[roomToMoveToId - 1].containsItem->itemName << " - " << maze[roomToMoveToId - 1].containsItem->itemDescription << endl;
            for (int i = 0; i < 4; i++) {
                if (player.itemsInInventory[i] == nullptr) {
                    player.itemsInInventory[i] = maze[roomToMoveToId - 1].containsItem;
                    maze[roomToMoveToId- 1].containsItem = nullptr;
                    break;
                }
            }
        }
        player.currentRoomId = roomToMoveToId;
        if (maze[player.currentRoomId - 1].finalRoom) {
            cout << "You found the exit!" << endl;
            goto you_won_exit_game;
        }
        cout << "You enter a room." << endl;
        goto ask_direction;
    } else {
        cout << "To the "<< directionAsWord <<" of you is a wall. There's got to be another way..." << endl;
        goto ask_direction;
    }
    

// print_current_position:
//     for (int i = 0; i < roomAmount; i++) {
//         cout << endl;
//         cout << maze[i].roomName << " - " << maze[i].id << endl;
//         if (maze[i].finalRoom) {
//             cout << "Final Room!" << endl;
//         }
//         if (maze[i].containsItemId) {
//             cout << "Has item: " << availableItems[4 - maze[i].containsItemId].itemName << endl;
//         }
//         if (maze[i].requiredItemId) {
//             cout << "Requires item: " << availableItems[4 - maze[i].requiredItemId].itemName << endl;
//         }
//         cout << "North: "<< to_string(maze[i].roomToTheNorth) << endl;
//         cout << "East: "<< to_string(maze[i].roomToTheEast) << endl;
//         cout << "South: "<< to_string(maze[i].roomToTheSouth) << endl;
//         cout << "West: "<< to_string(maze[i].roomToTheWest) << endl;
//     }

you_won_exit_game:
    delete []maze;

    return 0;
}