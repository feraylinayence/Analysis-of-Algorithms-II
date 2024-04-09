/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 2
 * Feray Lina Yence
 * 150190007
*/

#include <iostream>   // For input/output
#include <cmath>      // For distance calculations
#include <vector>     // For vector operations
#include <algorithm>  // For sorting operations
#include <limits>     // For min(), max() functions
#include <fstream>    // To read/write files
#include <chrono>     // For time measurement

//(These libraries are sufficient for this assignment.)

using namespace std;

// Structure to represent a point in 2D space
struct Point {
    double x, y;
};

// Helper function to calculate distance between two points
double distance(Point p1, Point p2) {

    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));

}

// Helper function to compare points based on x-coordinate, can use this for sorting.
bool compareX(Point p1, Point p2) {

    //Will indicate if the first point's x-coordinate is smaller
    return p1.x < p2.x;

}

// Helper function to compare points based on y-coordinate, can use this for sorting.
bool compareY(Point p1, Point p2) {

    //Will indicate if the first point's y-coordinate is smaller
    return p1.y < p2.y;

}

// Function to find the closest pair of points using brute force.
pair<Point, Point> bruteForceClosestPair(vector<Point>& points, int start, int end) {

    pair<Point, Point> closestPair;
    double min_dist = numeric_limits<double>::max();

    for(int i = start; i < end; i++){
        for(int j = i + 1; j <= end; j++){
            double dist = distance(points[i], points[j]);
            if(dist < min_dist){
                min_dist = dist; 
                closestPair = make_pair(points[i], points[j]);
            }
        }
    }

    return closestPair;

}

// The main objective of the assignment. This is the function to find the closest pair of points recursively using divide and conquer.
pair<Point, Point> closestPair(vector<Point>& points, int start, int end) {

    //return bruteForceClosestPair(points, start, end);

    if(end - start <= 2){
        return bruteForceClosestPair(points, start, end);
    }

    int middle = (start + end) / 2;
    //Closest pairs of the two halves
    auto left_pair = closestPair(points, start, middle);
    auto right_pair = closestPair(points, middle, end);

    auto closest_pair = distance(left_pair.first, left_pair.second) < distance(right_pair.first, right_pair.second) ? left_pair : right_pair;

    double min_dist = distance(closest_pair.first, closest_pair.second);

    vector<Point> strip;
    //Left and right points that are max min_dist away from the middle
    for(int i = start; i <= end; i++){
        if(abs(points[i].x - points[middle].x) < min_dist){
            strip.push_back(points[i]);
        }
    }

    //Sorting in ascending order of y-coordinate
    sort(strip.begin(), strip.end(), compareY);

    for(size_t i = 0; i < strip.size(); ++i){//MAKE IT 15 POINTS!!!
        for(size_t j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < min_dist && j < i + 16; ++j){
            double dist = distance(strip[i], strip[j]);
            if(dist < min_dist){
                min_dist = dist;

                closest_pair = make_pair(strip[i], strip[j]);
            }
        }
    }

    return closest_pair;
}

// Function to remove a pair of points from the point vector. Returns the newly formed vector.
vector<Point> removePairFromVector(vector<Point>& point_vector, pair<Point,Point> point_pair) {

    vector<Point> new_vector;

    for(auto it = point_vector.begin(); it != point_vector.end(); ++it){
        if(!(it->x == point_pair.first.x && it->y == point_pair.first.y)) {
            if(!(it->x == point_pair.second.x && it->y == point_pair.second.y)){
                new_vector.push_back(*it);
            }
        }
    }
    return new_vector;

}

// This function should wrap up the entire algorithm. It should:
//    1- Find the closest pair
//    2- Add the pair to the "pairs" vector
//    3- Remove the paired result from the map
// at each iteration.
// Then it should set "unconnected" variable as the final, unconnected city (if it exists).
void findClosestPairOrder(vector<Point> points) {
    vector<pair<Point,Point>> pairs; //add your pairs to this vector
    Point unconnected = {-1,-1}; //set this as the unconnected point if it exists, leave as is otherwise.

    while(!points.empty()){
        if(points.size() == 1){
            unconnected = points.back();
            points.pop_back();
        }
        else{
            sort(points.begin(), points.end(), compareX);
            auto closest_pair = closestPair(points, 0, points.size()-1);
            if(closest_pair.first.y > closest_pair.second.y){
                closest_pair = make_pair(closest_pair.second, closest_pair.first);
            }
            pairs.push_back(closest_pair);
            points = removePairFromVector(points, closest_pair);
        }
    }

    //before printing, please make sure that within each pair, the city with the smaller y coordinate is printed first...
    //...if the y coordinates are the same, print the city with the smaller x coordinate first.

    //This part prints the output, don't modify.    
    for(size_t i = 0; i < pairs.size(); i++){
        cout << "Pair " << i+1 << ": " << pairs[i].first.x << ", " << pairs[i].first.y << " - " << pairs[i].second.x << ", " << pairs[i].second.y << endl;
    }
    if (unconnected.x != -1){
        cout << "Unconnected " << unconnected.x << ", " << unconnected.y;
    }

}

//Read the coordinates from the file and convert them to a vector. Return the vector as a "vector<Point>"
vector<Point> readCoordinatesFromFile(const string& filename) {
    vector<Point> points;

    ifstream file(filename);

    if(!file.is_open()){
        cerr << "Error: Unable to open file " << filename << endl;
    }

    double x, y;
    while(file >> x >> y){
        Point new_point = {x, y};
        points.push_back(new_point);
    }

    file.close();

    return points;
}

// Main function. Ideally, you don't need to modify this.
int main(int argc, char* argv[]) {
    vector<Point> points = readCoordinatesFromFile(argv[1]);
    findClosestPairOrder(points);
    return 0;
}
