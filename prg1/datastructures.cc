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
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::station_count()
{
    // Replace the line below with your implementation
    return Stations.size();
}


//ei toimi tällä hetkellä; poistaa yhden itemin
void Datastructures::clear_all()
{
    // Replace the line below with your implementation
    Stations.clear();
}

std::vector<StationID> Datastructures::all_stations()
{
    // Replace the line below with your implementation
    std::vector<StationID> station_ids = {};
    for(const auto& station : Stations){
        station_ids.push_back(Stations[station.first]->id_);
    }
    return station_ids;
}

bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    // Replace the line below with your implementation
    if(Stations.find(id)==Stations.end()){
        Station *station = new Station{id, name, xy};
        Stations.insert({id,station});
        station_names.insert({name,id});
        add_distance(station);
        return true;
    }
    else{
        return false;
    }
}


void Datastructures::add_distance(Station* newstat){
    double dist = sqrt((newstat->coords_.x * newstat->coords_.x) + (newstat->coords_.y * newstat->coords_.y));
    station_dists.insert({dist,newstat->id_});
}


Name Datastructures::get_station_name(StationID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )

    if(Stations.find(id)==Stations.end()){return NO_NAME;}
    else{return Stations[id]->name_;}
}

Coord Datastructures::get_station_coordinates(StationID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
//add check for false id?
    if(Stations.find(id)==Stations.end()){
        return NO_COORD;
    }
    else{
        return Stations[id]->coords_;
    }
}


std::vector<StationID> Datastructures::stations_alphabetically()
{
    // Replace the line below with your implementation
    std::vector<StationID> ret = {};
    for(auto& i : station_names){
        ret.push_back(i.second);
    }
    return ret;
}


std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> stats_dist_increasing ={};
    for(auto& stat : station_dists){
        stats_dist_increasing.push_back(stat.second);
    }
    return stats_dist_increasing;
}

StationID Datastructures::find_station_with_coord(Coord xy)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )

    for(const auto& stat : Stations){
        if(Stations[stat.first]->coords_.x==xy.x and Stations[stat.first]->coords_.y==xy.y){
            return Stations[stat.first]->id_;
        }
    }
    return NO_STATION;

}

bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    // Replace the line below with your implementation
    if(Stations.find(id)==Stations.end()){

        return false;;
    }
    else{
        Stations[id]->coords_ = newcoord;
        return true;
    }
}


bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(Stations.find(stationid)==Stations.end()){
        return false;
    }
    else{
        std::pair<StationID,Time> topi = std::make_pair(trainid,time);
        Stations[stationid]->departures_.push_back(topi);
        return true;
    }
}


bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(Stations.find(stationid)==Stations.end()){
        return false;
    }
    else{
        for(unsigned int i=0;i<Stations[stationid]->departures_.size();++i){
            if(Stations[stationid]->departures_.at(0).first==trainid and Stations[stationid]->departures_.at(0).second==time){
                Stations[stationid]->departures_.erase(Stations[stationid]->departures_.begin()+i);
                return true;
            }
        }
        return false;
    }
}

//funktion pitää järjestää lähdöt aikajärjestykseen
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    // Replace the line below with your implementation
    std::vector<std::pair<Time,TrainID>> stat_deps = {};
    if(Stations.find(stationid)==Stations.end()){
        std::pair<Time,TrainID> errorpair = std::make_pair(NO_TIME,NO_STATION);
        stat_deps.push_back(errorpair);
    }
    else{
        for(const auto& dep : Stations[stationid]->departures_){
            if(dep.second>time){
                std::pair<Time,TrainID> pb = std::make_pair(dep.second,dep.first);
                stat_deps.push_back(pb);
            }
        }
    }
    return stat_deps;
}


bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    // Replace the line below with your implementation
    if(Regions.find(id)!=Regions.end()){
        return false;
    }
    else{
        Region *regali = new Region{id,name,coords};
        Regions.insert({id,regali});
        return true;
    }
}

std::vector<RegionID> Datastructures::all_regions()
{
    // Replace the line below with your implementation
    std::vector<RegionID> all_regs ={};
    for(const auto& reg : Regions){
        all_regs.push_back(reg.first);
    }
    return all_regs;
}

Name Datastructures::get_region_name(RegionID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(Regions.find(id)==Regions.end()){
        return NO_NAME;
    }
    else{
        return Regions[id]->rname_;
    }
}

std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(Regions.find(id)==Regions.end()){
        std::vector<Coord> r = {NO_COORD};
        return r;
    }
    else{
        return Regions[id]->rcoords_;
    }
}

// ei ota huomioon, mikkäli masterilla on masteri
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(Regions.find(parentid)==Regions.end()){
        return false;
    }
    else{
        if(Regions.find(id)==Regions.end()){
            return false;
        }
        else{
            Regions[parentid]->sub_regions.push_back(Regions[id]);
            Regions[id]->parent_ = Regions[parentid];

            return true;
        }
    }
}

bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(Regions.find(parentid)==Regions.end() or Stations.find(id)==Stations.end()){
        return false;
    }
    else{
        Station* newstation = new Station{id};
        Regions[parentid]->reg_stations.insert({id,newstation});
        return true;

    }
}

std::vector<RegionID> Datastructures::recu(Region* reg,std::vector<RegionID>& path){
    path.push_back(reg->rid_);
    if(!reg->parent_){
        return path;
    }
    return recu(reg->parent_,path);
}


std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    std::vector<RegionID> stat_in_regs ={};
    if(Stations.find(id)==Stations.end()){
        stat_in_regs.push_back(NO_REGION);
        return stat_in_regs;
    }
    else{
        for(auto reg :Regions){
            if(reg.second->reg_stations.find(id)!=reg.second->reg_stations.end()){

                auto* ptr = reg.second;
                while(ptr){
                    stat_in_regs.push_back(ptr->rid_);
                    ptr = ptr->parent_;
                }

                //recu(reg.second,stat_in_regs);
            }
        }
        return stat_in_regs;
    }
}

void Datastructures::allsubofreg(Region* p,std::vector<RegionID>& allsubregs){

    //for region in p:s subregion
    for(const auto& reg : p->sub_regions){

        //add region id to vec
        allsubregs.push_back(reg->rid_);

        //go through subregs recursively
        //with region
        allsubofreg(reg,allsubregs);
    }
    return;
}
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    std::vector<RegionID> allsubregs = {};
    allsubofreg(Regions[id],allsubregs);
    return allsubregs;
}

std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    std::vector<StationID> closest_stat_ids={};
    std::map<int,StationID> closest={};
    for(const auto& stat : Stations){
        double dist = sqrt(((stat.second->coords_.x - xy.x) * (stat.second->coords_.x - xy.x)) + ((stat.second->coords_.y - xy.y) * (stat.second->coords_.y - xy.y)));
        closest.insert({dist,stat.second->id_});
    }
    for(const auto& i : closest){
        if(closest_stat_ids.size()==3){break;}
        else{closest_stat_ids.push_back(i.second);}
    }
    return closest_stat_ids;
}


//lisää kaikista tietorakenteista poisto
bool Datastructures::remove_station(StationID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(Stations.find(id)==Stations.end()){
        return false;
    }
    else{
        Stations.erase(id);
        return true;
    }
}


bool Datastructures::dequrecu(Region* reg,std::deque<RegionID>& path){
    path.push_front(reg->rid_);
    if(reg->parent_ == nullptr){return false;}
    else{dequrecu(reg->parent_,path);}
    return true;
}

RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    std::deque<RegionID> path1 = {};
    std::deque<RegionID> path2 = {};
    dequrecu(Regions[id1],path1);
    dequrecu(Regions[id2],path2);
    int range = (path1.size()>path2.size()) ? path1.size() : path2.size();
    for(int i=0;i<range;++i){
        if(path1.at(i)==path2.at(i)){
            return i;
        }
    }
    return NO_REGION;
}
