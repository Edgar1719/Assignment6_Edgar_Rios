#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "coordinate.h"


int main(int argc, char *argv[])
{
    // Check that the user entered the number of coordinates
    if(argc != 2)
    {
        std::cout << "Usage: ./main number_of_coordinates" << std::endl;
        return 1;
    }

    int number_of_coordinates = std::atoi(argv[1]);

    if(number_of_coordinates <= 0)
    {
        std::cout << "Please enter a positive number." << std::endl;
        return 1;
    }

    // Start with head and tail pointing to nothing
    Coordinate *head = nullptr;
    Coordinate *tail = nullptr;

    // Generate random x and y coordinates
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for(int i = 0; i < number_of_coordinates; i++)
    {
        float x = (std::rand() % 201 - 100) / 10.0f;
        float y = (std::rand() % 201 - 100) / 10.0f;

        if(head == nullptr)
        {
            // Create the first coordinate
            head = new Coordinate;
            head->x = x;
            head->y = y;
            head->coord_id = 1;
            head->next = nullptr;
            head->previous = nullptr;

            // Both head and tail point to the first coordinate
            tail = head;
        }
        else
        {
            // Add a coordinate after the tail and move tail forward
            add_coordinate(tail, x, y);
            tail = tail->next;
        }
    }

    // Display the original list in both directions
    std::cout << "List length: " << list_length(head) << std::endl;

    std::cout << "\nForward Display:" << std::endl;
    forward_display(head);

    std::cout << "\nBackward Display:" << std::endl;
    backward_display(tail);

    // Find the coordinate closest to the point (0, 0)
    closest_to(head, 0.0f, 0.0f);

    // Use a temporary placeholder so the first coordinate can be deleted
    Coordinate placeholder = {};
    placeholder.next = head;
    head->previous = &placeholder;

    // Delete ID 3 if present; otherwise delete the first coordinate
    int coord_id_to_delete = number_of_coordinates >= 3 ? 3 : 1;
    delete_coordinate(&placeholder, coord_id_to_delete);

    // Restore the head pointer and remove the temporary placeholder
    head = placeholder.next;
    if(head != nullptr)
    {
        head->previous = nullptr;
    }

    // Update tail in case the last coordinate was deleted
    tail = head;
    while(tail != nullptr && tail->next != nullptr)
    {
        tail = tail->next;
    }

    // Display the remaining coordinates after deletion
    std::cout << "\nAfter Deleting Coordinate " << coord_id_to_delete << ":" << std::endl;
    std::cout << "List length: " << list_length(head) << std::endl;

    std::cout << "\nForward Display:" << std::endl;
    forward_display(head);

    std::cout << "\nBackward Display:" << std::endl;
    backward_display(tail);

    // Free the memory for all remaining coordinates
    Coordinate *current = head;

    while(current != nullptr)
    {
        Coordinate *next_coordinate = current->next;
        delete current;
        current = next_coordinate;
    }

    return 0;
}


// Add a new coordinate to the end of the list
void add_coordinate(Coordinate *list_end, float x, float y)
{
    // Find the beginning of the list to determine its length
    Coordinate *list_beginning = list_end;

    while(list_beginning->previous != nullptr)
    {
        list_beginning = list_beginning->previous;
    }

    // Allocate memory and assign the coordinate values
    Coordinate *new_coordinate = new Coordinate;
    new_coordinate->x = x;
    new_coordinate->y = y;
    new_coordinate->coord_id = list_length(list_beginning) + 1;

    // Connect the new coordinate to the current tail
    new_coordinate->previous = list_end;
    new_coordinate->next = nullptr;
    list_end->next = new_coordinate;
}


// Display coordinates from beginning to end
void forward_display(Coordinate *list_beginning)
{
    Coordinate *current = list_beginning;

    while(current != nullptr)
    {
        std::cout << "ID: " << current->coord_id
                  << "  X: " << current->x
                  << "  Y: " << current->y << std::endl;

        current = current->next;
    }
}


// Display coordinates from end to beginning
void backward_display(Coordinate *list_end)
{
    Coordinate *current = list_end;

    while(current != nullptr)
    {
        std::cout << "ID: " << current->coord_id
                  << "  X: " << current->x
                  << "  Y: " << current->y << std::endl;

        current = current->previous;
    }
}


// Delete the selected coordinate from the linked list
void delete_coordinate(Coordinate *list_beginning, int coord_id_to_delete)
{
    // list_beginning is the temporary placeholder for deletion
    Coordinate *temp = list_beginning->next;

    // Find the coordinate with the selected ID
    while(temp != nullptr && temp->coord_id != coord_id_to_delete)
    {
        temp = temp->next;
    }

    if(temp == nullptr)
    {
        std::cout << "Coordinate not found." << std::endl;
        return;
    }

    // Reconnect the coordinates before and after the deleted node
    temp->previous->next = temp->next;

    if(temp->next != nullptr)
    {
        temp->next->previous = temp->previous;
    }

    // Free the memory used by the removed coordinate
    delete temp;
}


// Count the number of coordinates in the linked list
int list_length(Coordinate *list_beginning)
{
    int count = 0;
    Coordinate *current = list_beginning;

    while(current != nullptr)
    {
        count++;
        current = current->next;
    }

    return count;
}


// Find the coordinate closest to a given x and y point
void closest_to(Coordinate *list_beginning, float x, float y)
{
    if(list_beginning == nullptr)
    {
        std::cout << "The list is empty." << std::endl;
        return;
    }

    // Start with the first coordinate as the closest point
    Coordinate *closest = list_beginning;
    float dx = closest->x - x;
    float dy = closest->y - y;
    float closest_distance = std::sqrt(dx * dx + dy * dy);

    // Compare the distance to each remaining coordinate
    Coordinate *current = list_beginning->next;

    while(current != nullptr)
    {
        dx = current->x - x;
        dy = current->y - y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if(distance < closest_distance)
        {
            closest = current;
            closest_distance = distance;
        }

        current = current->next;
    }

    // Display the closest coordinate and the distance to it
    std::cout << "\nClosest to (" << x << ", " << y << "):" << std::endl;
    std::cout << "ID: " << closest->coord_id
              << "  X: " << closest->x
              << "  Y: " << closest->y << std::endl;
    std::cout << "Distance: " << closest_distance << std::endl;
}

