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
};

struct Room {
    int id;
    string roomName;
    int roomToTheNorth;
    int roomToTheEast;
    int roomToTheSouth;
    int roomToTheWest;
    int requiredItemId;
    int containsItemId;
    bool finalRoom;
};

struct Player {
    int currentRoomId;
    int itemsInInventory[4];
    string pathTaken;
};

int main()
{

    goto initialize_player; 

initialize_player:
    Player player = {
        0,
        {0, 0, 0, 0},
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
            0, // roomToTheNorth
            0, // roomToTheEast
            0, // roomToTheSouth
            0, // roomToTheWest
            0, // requiredItemId
            0, // containsItemId
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
        "Locked", // Needs key - exit
        "Creepy", // Needs newspaper - Has key
        "Secret", // Needs secret note - Has newspaper
        "Dark", // Needs torch - Has note
    };

    Item availableItems[4] = {
        { 4, "Key" },
        { 3, "Rolled-up newspaper" },
        { 2, "Secret note" },
        { 1, "Torch" },
    };

    int addedItems = 0;
    int maxAdjecent = 1;

    int roomsAdjecentToFirst[4] = {maze[0].roomToTheNorth, maze[0].roomToTheEast, maze[0].roomToTheSouth, maze[0].roomToTheWest};
    for (int i = 0; i < 4; i++) {
        if (roomsAdjecentToFirst[i] != 0) {
            // Assign the first item to a room that won't be locked and is next to the starting point
            maze[roomsAdjecentToFirst[i] - 1].containsItemId = availableItems[3].id;
            maze[roomsAdjecentToFirst[i] - 1].roomName = firstItemRoomName;
            break;
        }
    }

add_items_to_rooms:
    // Start from the end, since the less populated rooms are at the end of the maze array
    // Don't add item or requirement to the first room
    for (int i = roomAmount - 1; i > 1; i--) {
        // skip already assigned rooms
        if (maze[i].containsItemId != 0 || maze[i].requiredItemId != 0) continue;
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
            maze[i].containsItemId = availableItems[addedItems - 1].id;
        }
        maze[i].roomName = conditionalRoomAdjectives[addedItems];
        maze[i].requiredItemId = availableItems[addedItems].id;
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


print_current_position:
    for (int i = 0; i < roomAmount; i++) {
        cout << endl;
        cout << maze[i].roomName << " - " << maze[i].id << endl;
        if (maze[i].finalRoom) {
            cout << "Final Room!" << endl;
        }
        if (maze[i].containsItemId) {
            cout << "Has item: " << availableItems[4 - maze[i].containsItemId].itemName << endl;
        }
        if (maze[i].requiredItemId) {
            cout << "Requires item: " << availableItems[4 - maze[i].requiredItemId].itemName << endl;
        }
        cout << "North: "<< to_string(maze[i].roomToTheNorth) << endl;
        cout << "East: "<< to_string(maze[i].roomToTheEast) << endl;
        cout << "South: "<< to_string(maze[i].roomToTheSouth) << endl;
        cout << "West: "<< to_string(maze[i].roomToTheWest) << endl;
    }

    delete []maze;

    return 0;
}