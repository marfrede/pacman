#ifndef Orientation_hpp
#define Orientation_hpp
enum class Orientation { North, East, South, West };
#define orientationToString(o) (o == Orientation::North? "North" : (o == Orientation::East? "East" : (o == Orientation::West? "West" : "South")))
#endif /* Orientation_hpp */