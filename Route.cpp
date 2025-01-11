/*****************************************
** File: Route.cpp
** Project: CMSC 202 Project 3, Spring 2024
** Author: John Jo
** Date: 3/26/24
** Section: 34
** E-mail: sunghyj1@umbc.edu
**
** This file contains the linked list class called Route which
** has four peices of data: a name for the route (m_name),
** an Airport pointer tracking the front of the Route (m_head),
** an Airport pointer tracking the end of the Route (m_tail),
** and an integer tracking the size of the Route (m_size).
**
**
***********************************************/
#include "Route.h"

Route :: Route() {
    m_head = nullptr;
    m_tail = nullptr;
    m_size = 0;
}

void Route :: SetName(string name) {
    m_name = name;
}

Route :: ~Route() {
    while (m_head != nullptr) {
        Airport* temp = m_head;
        m_head = m_head->GetNext();
        delete temp;
    }
}

void Route :: InsertEnd(string code, string name, string city, string country, double north, double west) {
    // create a new Airport object with the provided data
    Airport* NewAirport = new Airport(code, name, city, country, north, west);
    // if the route is empty, set both head and tail to the new airport
    if (m_head == nullptr) {
        m_head = NewAirport;
        m_tail = NewAirport;
    } else {
        // if the route is not empty, insert the new airport at the end and update tail
        m_tail->SetNext(NewAirport);
        m_tail = NewAirport;
    }
    m_size++;
}

void Route :: RemoveAirport(int airport) {
    // check if the given airport index is invalid
    if (airport <= 0 || airport > m_size) {
        cout << "Invalid Route" << endl;
        return;
    }
    cout << UpdateName() << endl;
    Airport* curr = m_head;
    Airport* prev = m_head;
    int counter = 1;
    // if the airport to be removed is the first one
    if (airport == 1) {
        m_head = curr->GetNext();
        delete curr;
        m_size--;

        if (m_size == 0) {
            m_tail = nullptr;
        }
        return;
    }
    // traverse the route to find the airport to be removed
    while (curr != nullptr) {
        if (counter == airport) {
            // update the next pointer of the previous airport to skip the current airport
            prev->SetNext(curr->GetNext());
            // if the removed airport was the last one, update m_tail
            if (curr == m_tail) {
                m_tail = prev;
            }
            delete curr;
            m_size--;
            return;
        }
        // move to the next airport
        prev = curr;
        curr = curr->GetNext();
        counter++;
    }
}

string Route :: GetName() {
    return m_name;
}

string Route :: UpdateName() {
    // check if the route has less than 2 airports
    if (m_size < 2) {
        cout << "Route must have at least two airports." << endl;
        return "";
    }
    return m_head->GetCity() + " to " + m_tail->GetCity();
}

int Route :: GetSize() {
    return m_size;
}

void Route :: ReverseRoute() {
    // check if the route has less than 2 airports
    if (m_size < 2) {
        cout << "Route must have at least two airports." << endl;
        return;
    }
    Airport* prev = nullptr;
    Airport* current = m_head;
    Airport* next = nullptr;
    // reverse the links between airports in the route
    while (current != nullptr) {
        next = current->GetNext();
        current->SetNext(prev);
        prev = current;
        current = next;
    }
    // update the head and tail pointers after route is reversed
    m_tail = m_head;
    m_head = prev;
}

Airport* Route :: GetData(int index) {
    // check if the index is out of bounds
    if (index < 0 || index >= m_size) {
        cout << "Invalid index." << endl;
        return nullptr;
    }
    Airport* current = m_head;
    // traverse the route to find the airport at the index
    for (int i = 0; i < index; i++) {
        current = current->GetNext();
    }
    return current;
}

void Route :: DisplayRoute() {
    // check if the route is empty
    if (m_size == 0) {
        cout << "Route is empty." << endl;
        return;
    }
    int index = 1;
    Airport* current = m_head;
    // traverse the route and display information of each airport
    while (current != nullptr) {
        cout << index << ". " << current->GetCode() << ", " << current->GetName() << ", " 
             << current->GetCity() << ", " << current->GetCountry() << " (N" 
             << current->GetNorth() << " W" << current->GetWest() << ")" << endl;
        // move to the next airport in the route and increment index for next airport
        current = current->GetNext();
        index++;
    }
}
