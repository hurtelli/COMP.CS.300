// Datastructures.hh
//
// Student name: Otto Palmgren
// Student email: otto.palmgren@tuni.fi
// Student number: 50327953

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>

#include <map>
#include <deque>
#include <memory>


// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator>(Coord c1, Coord c2){return c1.x>c2.x && c1.y>c2.y;}
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Constant: Returns a struct attribute
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: 5 Linear iterations. O(5n) = O(n)
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear: Iteration through map
    std::vector<StationID> all_stations();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: 4 log(n) operations on map
                                    //O(4Log(n)) = O(log(n))
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: One find on map
    Name get_station_name(StationID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: One find on map
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: iterating through a map
                                    //and pushing_back vector O(1)
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: iterating through a map
                                    //pushing back vector O(1)
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: finding from a map
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: one map.find()
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: one map.find()
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: highest term is O(log(n))
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: highest term from iterating thourgh vec
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: map.find() tai map.insert()
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: iter through a map, pback a vec
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: one map.find()
    Name get_region_name(RegionID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: one map.find()
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Highest term in the function is O(n) but kinda not
                                    //map.find()
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: map.insert() | map.find()
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: highest term is O(n), but the n
                                    //isn't the n, but instead k region
                                    //parent amount.
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Has actually a good complexity, it is recursive but not O(n)
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Very slow complexity, due to loop of O(n) which
                                        //has O(log(n)) on every occasion.
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: highest term 2*log(n)
    bool remove_station(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: highest term is 3n
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

private:

//TODO:
    //TARKISTA COMPULSORY aika
    //ÄDÄÄMISEEN MENEE HELVETISTI AIKAA PASKA ON RIKKI
        //Todennäkösesti kyse add_sub2reg uudesta muutoksesta lineaarisella find
            //subregions vectorista mappiin?

    struct Region;

    struct Station{
        StationID id_="";
        Name name_=NO_NAME;
        Coord coords_=NO_COORD;
        int dist_ = NO_VALUE;

        std::multimap<Time,StationID> departures_ = {};

        std::shared_ptr<Region> in_reg_ = nullptr;

    };

    struct Region{
        RegionID rid_ = 0;
        Name rname_=NO_NAME;
        std::vector<Coord> rcoords_={NO_COORD};

        std::unordered_map<RegionID,std::shared_ptr<Region>> sub_regions_ ={};
        std::unordered_map<StationID,std::shared_ptr<Station>> reg_stations_ = {};
        //std::deque<RegionID> reg_path_ = {};

        std::shared_ptr<Region> parent_ = nullptr;
    };


    // Add stuff needed for your class implementation here
    std::unordered_map<StationID,std::shared_ptr<Station>> Stations;
    std::multimap<Name,StationID> stat_names;
    std::multimap<Coord,StationID> stat_coords;


    std::multimap<unsigned int,StationID> stat_dists;

    std::unordered_map<RegionID,std::shared_ptr<Region>> Regions;

    unsigned int distance(Coord&);
    void allsubofreg(std::shared_ptr<Region>,std::vector<RegionID>&);
    std::vector<RegionID> recu(std::shared_ptr<Region>,std::vector<RegionID>&);
    void dequrecu(std::shared_ptr<Region>,std::deque<RegionID>&);
    RegionID iter_forward(std::shared_ptr<Region>,std::vector<RegionID>&);
};

#endif // DATASTRUCTURES_HH
