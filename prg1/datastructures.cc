// Datastructures.cc
//
// Student name: Otto Palmgren
// Student email: otto.palmgren@tuni.fi
// Student number: 50327953

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <algorithm>



std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implementää lähdöt aikajärjestykseen
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)
/**
 * @brief constructor, no function as of now
 */
Datastructures::Datastructures()
{
    // Write any initialization you need here
}

/**
 * @brief Datastructures::~Datastructures destructor, calls clear_all()
 */
Datastructures::~Datastructures()
{
    clear_all();
}

/**
 * @brief station_count returns the amount of entries in Stations
 * @return Stations entries amount
 */
unsigned int Datastructures::station_count()
{
    return Stations.size();
}


/**
 * @brief clear_all empties out all containers and destroys objects sharedptrs
 */
void Datastructures::clear_all()
{

    stat_names.clear(); //O(n)
    stat_coords.clear();//O(n)
    stat_dists.clear(); //O(n)
    /*
    for(auto& stat : Stations){
        stat.second->in_reg_=nullptr;
    }
    */
    Stations.clear();   //O(n)
    for(auto& reg : Regions){   //O(n)
        reg.second->parent_=nullptr;
        reg.second->sub_regions_.clear();
        reg.second->reg_stations_.clear();
    }
    Regions.clear();    //O(n)

}


//performance 10/10
/**
 * @brief all_stations, gathers all station ids to a vector
 * @return vector with all station ids
 */
std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> station_ids = {};
    for(const auto& station : Stations){    //O(n)
        station_ids.push_back(station.second->id_);    //O(1)
    }
    return station_ids;
}

/**
 * @brief add_station adds a station to needed datastructures
 * @param id is the id for the station being added
 * @param name is the name of the station with id id
 * @param xy are the coordinates of forementioned station
 * @return false if station already exists, true if not
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    auto i = Stations.find(id); //O(log(n))
    if(i==Stations.end()){
        unsigned int dist =distance(xy);
        std::shared_ptr<Station> station(new Station{id, name, xy,dist,{}});
        Stations.insert({id,station});  //O(log(n))
        stat_names.insert({name,id});    //O(log(n))
        stat_dists.insert({dist,id});  //O(log(n))
        stat_coords.insert({xy,id});
        return true;
    }
    else{
        return false;
    }
}

//performance-wise sqrt is slow
/**
 * @brief distance calculates the coordinates distance from 0,0
 * @param xy the coordinates to be calculated with
 * @return the distance
 */
unsigned int Datastructures::distance(Coord& xy){
    return sqrt((xy.x * xy.x) + (xy.y * xy.y));
}


//perftest station_info 6/10
//should be about const
/**
 * @brief get_station_name finds the name associated with station with id
 * @param id is the id of the station which name is being searched
 * @return the name of the station with id id
 */
Name Datastructures::get_station_name(StationID id)
{
    auto i = Stations.find(id); //O(log(n))
    if(i==Stations.end()){return NO_NAME;}
    else{
        return i->second->name_;
    }
}

//perftest station_info 6/10
//should be about const or log(n)?
/**
 * @brief get_station_coordinates finds the coordinates of a station
 * @param id is the id of the station which coordinates we search
 * @return the coordinates
 */
Coord Datastructures::get_station_coordinates(StationID id)
{
    auto i =Stations.find(id); //O(log(n))
    if(i==Stations.end()){return NO_COORD;}
    else{
        return i->second->coords_;
    }
}

//perftest 10/10
/**
 * @brief stations_alphabetically returns stations sorted alphabetically
 * @return stations sorted alphabetically,empty if no stations found
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<StationID> ret = {};
    for(const auto& stat : stat_names){   //O(n)
        ret.push_back(stat.second);
    }
    return ret;
}

//perftest 10/10
/**
 * @brief stations_distance_increasing returns stations sorted by distance
 * @return stations sorted by distance, empty if no stations found
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> stats_dist_increasing ={};
    for(auto& stat : stat_dists){   //O(n)
        stats_dist_increasing.push_back(stat.second);   //O(1)
    }
    return stats_dist_increasing;
}


//perftest 10/10
/**
 * @brief find_station_with_coord tries to find a station with given coords
 * @param xy the coords where we search for a station
 * @return the station id,NO_STATION if no station found
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
    auto stat = stat_coords.find(xy);   //O(log(n))
    if(stat==stat_coords.end()){return NO_STATION;}
    else{return stat->second;}

}

//perftest 10/10
/**
 * @brief change_station_coord changes a stations coordinates in datastructures
 * @param id is the id of the station which coordinates are going to be modified
 * @param newcoord are the new coordinates for the station in question
 * @return false if there is no station with id id, elsewise returns true
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    auto i = Stations.find(id); //O(log (n))
    if(i==Stations.end()){
        return false;;
    }
    else{
        stat_coords.erase(i->second->coords_);  //O(log(n))
        stat_coords.insert({newcoord,id});  //O(log(n))

        auto n = stat_dists.find(i->second->dist_);  //O(log(n))
        auto m = n;
        while(m->first==n->first and m->second!=id){
            ++n;
        }
        if(m->first==n->first and m->second==n->second){
            stat_dists.erase(n);    //O(1)
        }
        stat_dists.insert({distance(newcoord),id}); //O(log(n))

        i->second->coords_ = newcoord;
        return true;
    }
}

//perftest 10/10
/**
 * @brief add_departure adds a train departure for a station
 * @param stationid the station which the train is deaprting from
 * @param trainid the deaprting train id
 * @param time is the time the departing train departs
 * @return false if no station found with id, true elsewise
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    auto stat = Stations.find(stationid);   //O(log (n))
    if(stat==Stations.end()){
        return false;
    }
    else{
        stat->second->departures_.insert({time,trainid});   //O(log (n))
        return true;
    }
}


//perftest 10/10
/**
 * @brief remove_departure removes a trains departure from a station
 * @param stationid the stations id the departure is being removed from
 * @param trainid the trains id which is departing
 * @param time the time the train is departing
 * @return false if no station found, true elsewise
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    auto stat = Stations.find(stationid);   //O(log(n))
    if(stat==Stations.end()){
        return false;
    }
    else{
        auto dep = stat->second->departures_.find(time);    //O(log(n))
        if(dep==stat->second->departures_.end() or dep->second!=trainid){
            return false;
        }
        else{
            stat->second->departures_.erase(dep);  //O(1)
            return true;
        }
    }
}


//perftest 10/10
// too much commands?
/**
 * @brief station_departures_after gathers the departures from the station after a given time
 * @param stationid is the id of the station which deaprtures are going to be checked
 * @param time is the time after which the departures are going to be displayed
 * @return  a vector which has pairs of departure time and the dperating trainid
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    std::vector<std::pair<Time,TrainID>> stat_deps = {};
    auto stat = Stations.find(stationid);
    if(stat==Stations.end()){   //O(log (n))
        stat_deps.push_back({NO_TIME,NO_STATION}); //O(1)
    }
    else{
        auto lb = stat->second->departures_.lower_bound(time);    //O(log(n))
        while(lb!=stat->second->departures_.end()){ //O(n)
            stat_deps.push_back({lb->first,lb->second});
            ++lb;
        }
    }
    return stat_deps;
}


/**
 * @brief add_region adds a region to the datastructures
 * @param id is the regions id which is goin to be added
 * @param name is the name of the region
 * @param coords are the coords of the region
 * @return false if region already exists, true elsewise
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    if(Regions.find(id)!=Regions.end()){    //O(log (n))
        return false;
    }
    else{
        std::shared_ptr<Region> reg(new Region{id,name,coords,{},{},nullptr});
        Regions.insert({id,reg});   //O(log (n))
        return true;
    }
}

/**
 * @brief all_regions return all region ids in the datastructures
 * @return a vector with all region ids in the datastructures
 */
std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> all_regs ={};
    for(const auto& reg : Regions){ //O(n)
        all_regs.push_back(reg.first);  //O(1)
    }
    return all_regs;
}

//perftest region_info 6/10
//should be almost constant??
/**
 * @brief get_region_name gets the region with id id, name
 * @param id the regions id which name is being searched
 * @return the name of the region with id id, NO_NAME if no region found
 */
Name Datastructures::get_region_name(RegionID id)
{
    const auto& reg = Regions.find(id);    //O(log (n))
    if(reg==Regions.end()){
        return NO_NAME;
    }
    else{
        return reg->second->rname_;
    }
}

//perftest region_info 6/10
//should be almost constant??
/**
 * @brief get_region_coords returns coordinates of a region
 * @param id is the id of the region which coordinates we are after
 * @return a vector with the regions coordinates, if no region found vec only element is NO_COORD
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    if(Regions.find(id)==Regions.end()){    //O(log (n))
        return {NO_COORD};
    }
    else{
        return Regions[id]->rcoords_;
    }
}

/**
 * @brief add_subregion_to_region adds a subregion for a region in every needed datastructure
 * @param id the subregions id
 * @param parentid the parent regions id
 * @return false if no parentregion or subregion found, true elsewise
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    const auto& parent = Regions.find(parentid);  //O(log(n))
    if(parent==Regions.end()){
        return false;
    }
    else{ 
        const auto& newreg =Regions.find(id);    //O(log(n))
        if(newreg==Regions.end()){
            return false;
        }
        else{
            if(newreg->second->parent_){
                return false;
            }
            auto i = parent->second;
            while(i->parent_){
                if(i==newreg->second){
                    return false;
                }
                i=i->parent_;
            }
            parent->second->sub_regions_.push_back(newreg->second);
            newreg->second->parent_ = parent->second;
            return true;
        }

    }
}

/**
 * @brief add_station_to_region adds a station for a region in every needed datastructure
 * @param id the adding stations id
 * @param parentid the regions id which the station is put into
 * @return false if either the station or the region doesn't exist, true otherwise
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    auto preg = Regions.find(parentid); //O(log(n))
    auto stat = Stations.find(id);  //O(log(n))
    if(preg==Regions.end() or stat==Stations.end()){
        return false;
    }
    else{
        preg->second->reg_stations_.insert({id, Stations[id]});   //O(log(n))
        stat->second->in_reg_ = preg->second;
        return true;
    }
}



//could be faster
//perftest 6/10
//should be almost totally constant
/**
 * @brief station_in_regions finds all the regions which the station is in
 * @param id the stations id which parentregions are searched for
 * @return a vector with every parent region id, if no parent regions, or no station found
 *          only element will be NO_REGION
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    std::vector<RegionID> stat_in_regs ={};
    auto stat = Stations.find(id);  //O(log(n))
    if(stat==Stations.end()){
        return {NO_REGION};
    }
    else{
        auto ptr = stat->second->in_reg_;
        while(ptr){ //O(n of parents)
            stat_in_regs.push_back(ptr->rid_);  //O(1)
            ptr = ptr->parent_;
        }
        return stat_in_regs;
    }
}


/**
 * @brief allsubofreg goes through subregions of a region to add their id:s to a vector
 * @param p the region which is currently being checked
 * @param allsubregs a vector with the subregions ids
 */
void Datastructures::allsubofreg(std::shared_ptr<Region> p,std::vector<RegionID>& allsubregs){

    //for region in p:s subregion
    if(p->sub_regions_.size()==0){
        return;
    }
    else {
        for(const auto& reg : p->sub_regions_){//O(n)

            //add region id to vec
            allsubregs.push_back(reg->rid_);    //O(1)

            //go through subregs recursively
            //with region
            allsubofreg(reg,allsubregs);    //O(n)
        }
        return;
    }
}


//suht nopea
//perftest 10/10
/**
 * @brief all_subregions_of_region searches all subregions of a given region and its subregions subregions .... .. ...
 * @param id the id of the region which subregions we are going through
 * @return a vector with all the subregions ids
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    auto reg = Regions.find(id);
    if(reg==Regions.end()){
        return {NO_REGION};
    }
    else{
        std::vector<RegionID> allsubregs = {};
        allsubofreg(Regions[id],allsubregs);    //O(?)
        return allsubregs;
    }
}

/**
 * @brief fdist calculates the fast distance of two coordinates, WITHOUT a square root
 * @param c1 the first coordinates
 * @param c2 the second coordinates
 * @return the distances square
 */
int fdist(Coord c1,Coord c2){
    return (c1.x-c2.x)*(c1.x-c2.x) + (c1.y-c2.y)*(c1.y-c2.y);
}

//FUNKTIO EI OLE HYVÄ
//SAA HELPOSTI PARANNETTUA
//pitäis olla parempi kuin nlogn
//kikkaa stat_distsillä??
/**
 * @brief stations_closest_to searches for 3 closest stations to input coordinates
 * @param xy the coordinates of what we want to be closest to
 * @return a vector with 3 closest stations ids
 */
std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    std::vector<StationID> closest_stat_ids={};
    std::multimap<int,StationID> closest={};
    for(const auto& stat : Stations){   //O(n)
        int dist = fdist(stat.second->coords_,xy);
        closest.insert({dist,stat.second->id_});    //O(log(n))
    }

    for(const auto& stat : closest){    //O(3)
        if(closest_stat_ids.size()==3){
            break;
        }
        else{
            closest_stat_ids.push_back(stat.second);    //O(1)
        }
    }

    return closest_stat_ids;
}


//10/10perf
/**
 * @brief remove_station removes a station from needed datastructures
 * @param id the stations id going to be removed
 * @return false if no station found with id, true elsewise
 */
bool Datastructures::remove_station(StationID id)
{
    auto i = Stations.find(id); //O(log (n))
    if(i==Stations.end()){
        return false;
    }
    else{
        stat_names.erase(i->second->name_);    //O(log(n))

        //Instruction say that a case where two stats have the same coords
        //shouldn't be possible and should not be counted for
        stat_coords.erase(i->second->coords_);  //O(log(n))


        //Here we find stats with the same dists and go through
        //only them, to have better complexity
        //when finding the stat
        auto d = distance(i->second->coords_);
        auto n = stat_dists.find(d);  //O(log(n))
        if(n==stat_dists.end()){
            return false;
        }

        auto c = stat_dists.count(d);  //O(log(n))

        auto m=n;
        if(c<=1){
            stat_dists.erase(n);
        }
        else{
            while(m->first==d){ //O(k)
                if(m->second==id){
                    stat_dists.erase(m);  //O(log(n))
                    break;
                }
                ++m;
            }
        }


        Stations.erase(i);  //O(1)
        return true;
    }
}


/**
 * @brief dequrecu is a recursive function to go through regions parentregions and add their id to a deque
 * @param reg is a shared_ptr to the region which is currently being checked
 * @param path is a deque to contain the path from the starting region to the greatest parent region
 */
void Datastructures::dequrecu(std::shared_ptr<Region> reg,std::deque<RegionID>& path){
    path.push_front(reg->rid_);
    if(!reg->parent_){return;}
    else{dequrecu(reg->parent_,path);}
}


//perftest 10/10
/**
 * @brief common_parent_of_regions finds the closest common parent to 2 regions
 * @param id1 the 1 regions id
 * @param id2 the 2 regions id
 * @return the closest common parents id, if no common parent returns NO_REGION
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    auto reg1 = Regions.find(id1);
    auto reg2 = Regions.find(id2);
    if(reg1==Regions.end() or reg2==Regions.end()){
        return NO_REGION;
    }
    //deques to store path from a reg to the masterests reg
    std::deque<RegionID> path1 = {};
    std::deque<RegionID> path2 = {};

    //recursive finding out
    dequrecu(reg1->second,path1);   //O(n)
    dequrecu(reg2->second,path2);   //O(n)

    //range = bigger path size
    int range = (path1.size()>path2.size()) ? path1.size() : path2.size();
    for(int i=0;i<range;++i){   //O(n)
        if(path1.at(i)==path2.at(i)){
            return path1.at(i);
        }
    }
    return NO_REGION;
}
