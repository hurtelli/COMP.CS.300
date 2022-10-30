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

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    clear_all();
}

unsigned int Datastructures::station_count()
{
    return Stations.size();
}



void Datastructures::clear_all()
{
    Stations.clear();   //O(n)
    stat_names.clear(); //O(n)
    stat_dists.clear(); //O(n)
    Regions.clear();    //O(n)

}

//n=100  0.0031s
//n=1000  0.34s
//n=10000 >40s
//????!?!?!?!?!?!?!?!?!?!?!?!!?
std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> station_ids = {};
    for(const auto& station : Stations){    //O(n)
        station_ids.push_back(station.second->id_);    //O(1)
    }
    return station_ids;
}

bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    auto i = Stations.find(id); //O(log (n))
    if(i==Stations.end()){  //O(n)
        std::shared_ptr<Station> station(new Station{id, name, xy,{}});
        Stations.insert({id,station});  //O(log (n))
        stat_names.insert({name,id}); //O(log(n))
        stat_dists.insert({distance(xy),id});  //O(log(n))
        return true;
    }
    else{
        return false;
    }
}


unsigned int Datastructures::distance(Coord& xy){
    return sqrt((xy.x * xy.x) + (xy.y * xy.y));
}


Name Datastructures::get_station_name(StationID id)
{
    auto i = Stations.find(id); //O(log (n))
    if(i==Stations.end()){return NO_NAME;}
    else{
        if(i->second->name_==""){
            return NO_NAME;
        }

        else{
            std::string ret = Stations[id]->name_;
            return ret;
        }
    }
}

Coord Datastructures::get_station_coordinates(StationID id)
{

    auto i =Stations.find(id); //O(log (n))
    if(i==Stations.end()){
        return NO_COORD;
    }
    else{
        return i->second->coords_;
    }
}


std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<StationID> ret = {};
    for(auto& i : stat_names){  //O(n)
        ret.push_back(i.second);    //O(1)
    }
    return ret;
}


std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> stats_dist_increasing ={};
    for(auto& stat : stat_dists){   //O(n)
        stats_dist_increasing.push_back(stat.second);   //O(1)
    }
    return stats_dist_increasing;
}


//onko nopeempaa?
StationID Datastructures::find_station_with_coord(Coord xy)
{
    for(const auto& stat : Stations){   //O(n)
        auto i = Stations.find(stat.first); //O(log (n))
        if(i->second->coords_.x==xy.x and i->second->coords_.y==xy.y){
            return i->second->id_;
        }
    }
    return NO_STATION;

}


bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    auto i = Stations.find(id); //O(log (n))
    if(i==Stations.end()){
        return false;;
    }
    else{
        i->second->coords_ = newcoord;
        return true;
    }
}


bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    if(Stations.find(stationid)==Stations.end()){   //O(log (n))
        return false;
    }
    else{
        std::pair<StationID,Time> topi = std::make_pair(trainid,time);
        Stations[stationid]->departures_.push_back(topi);   //O(1)
        return true;
    }
}


//liian hidas
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{

    if(Stations.find(stationid)==Stations.end()){   //O(log(n))
        return false;
    }
    else{
        for(unsigned int i=0;i<Stations[stationid]->departures_.size();++i){ //O(n)
            if(Stations[stationid]->departures_.at(0).first==trainid and Stations[stationid]->departures_.at(0).second==time){
                Stations[stationid]->departures_.erase(Stations[stationid]->departures_.begin()+i); //O(log (n))
                return true;
            }
        }
        return false;
    }
}


//funktion pitää järjestää lähdöt aikajärjestykseen
//PASKA FUNKTIO
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    std::vector<std::pair<Time,TrainID>> stat_deps = {};
    if(Stations.find(stationid)==Stations.end()){   //=(log (n))
        std::pair<Time,TrainID> errorpair = std::make_pair(NO_TIME,NO_STATION);
        stat_deps.push_back(errorpair); //O(1)
    }
    else{
        for(const auto& dep : Stations[stationid]->departures_){    //O(n)
            if(dep.second>time){
                std::pair<Time,TrainID> pb = std::make_pair(dep.second,dep.first);
                stat_deps.push_back(pb);    //O(1)
            }
        }
    }
    return stat_deps;
}


//jostain syystä vitun hidas luokkaa 0.0002 joskus jopa 0.008
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    if(Regions.find(id)!=Regions.end()){    //O(log (n))
        return false;
    }
    else{
        //memoryloss tälle struct objectille koko ajan?
        std::shared_ptr<Region> reg(new Region{id,name,coords});
        Regions.insert({id,reg});   //O(log (n))
        return true;
    }
}

std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> all_regs ={};
    for(const auto& reg : Regions){ //O(n)
        all_regs.push_back(reg.first);  //O(1)
    }
    return all_regs;
}


Name Datastructures::get_region_name(RegionID id)
{
    if(Regions.find(id)==Regions.end()){    //O(log (n))
        return NO_NAME;
    }
    else{
        if(Regions[id]->rname_==""){
            return NO_NAME;
        }
        else{
            return Regions[id]->rname_;
        }
    }
}


std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    if(Regions.find(id)==Regions.end()){    //O(log (n))
        std::vector<Coord> r = {NO_COORD};
        return r;
    }
    else{
        return Regions[id]->rcoords_;
    }
}


bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    if(Regions.find(parentid)==Regions.end()){  //O(log (n))
        return false;
    }
    else{
        if(Regions.find(id)==Regions.end()){    //O(log (n))
            return false;
        }
        else{
            Regions[parentid]->sub_regions_.push_back(Regions[id]);
            Regions[id]->parent_ = Regions[parentid];

            return true;
        }
    }
}


bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    if(Regions.find(parentid)==Regions.end() or Stations.find(id)==Stations.end()){ //O(log (n))
        return false;
    }
    else{
        /*  EI TARVETTA?
        std::shared_ptr<Station> newstation(new Station{id,"",{},{}});
        Stations.insert({id,newstation});
        */
        Regions[parentid]->reg_stations_.insert({id, Stations[id]});   //O(log (n))
        return true;
    }
}



//could be faster
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    std::vector<RegionID> stat_in_regs ={};
    if(Stations.find(id)==Stations.end()){  //O(log (n))
        stat_in_regs.push_back(NO_REGION);  //O(1)
        return stat_in_regs;
    }
    else{
        for(auto& reg :Regions){    //O(n)
            if(reg.second->reg_stations_.find(id)!=reg.second->reg_stations_.end()){    //O(log (n))

                std::shared_ptr<Region> ptr = reg.second;
                while(ptr){ //O(n)?
                    stat_in_regs.push_back(ptr->rid_);  //O(1)
                    ptr = ptr->parent_;
                }
            }
        }
        return stat_in_regs;
    }
}


//vois olla parempi?
void Datastructures::allsubofreg(std::shared_ptr<Region> p,std::vector<RegionID>& allsubregs){

    //for region in p:s subregion
    if(p){
        for(const auto& reg : p->sub_regions_){//O(n)

            //add region id to vec
            allsubregs.push_back(reg->rid_);    //O(1)

            //go through subregs recursively
            //with region
            allsubofreg(reg,allsubregs);    //O(n)
        }
    }
    return;
}


//suht nopea
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    std::vector<RegionID> allsubregs = {};
    allsubofreg(Regions[id],allsubregs);    //O(?)
    return allsubregs;
}


//FUNKTIO EI OLE HYVÄ
//SAA HELPOSTI PARANNETTUA
std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    std::vector<StationID> closest_stat_ids={};
    std::map<int,StationID> closest={};
    for(const auto& stat : Stations){   //O(n)
        double dist = sqrt(((stat.second->coords_.x - xy.x) * (stat.second->coords_.x - xy.x)) + ((stat.second->coords_.y - xy.y) * (stat.second->coords_.y - xy.y)));
        closest.insert({dist,stat.second->id_});    //O(log (n))
    }
    for(const auto& i : closest){   //O(n))
        if(closest_stat_ids.size()==3){break;}
        else{closest_stat_ids.push_back(i.second);} //O(log (n))
    }
    return closest_stat_ids;
}


bool Datastructures::remove_station(StationID id)
{
    auto i = Stations.find(id); //O(log (n))
    if(i==Stations.end()){
        return false;
    }
    else{
        for(auto& stat : stat_dists){   //O(n)
            if(stat.second==id){stat_dists.erase(stat.first);break;}    //O(log (n))
        }

        for(auto& stat : stat_names){   //O(n)
            if(stat.second==id){stat_names.erase(stat.first);break;}  //O(log (n))
        }

        //invalid read of size 8
        Stations.erase(i);  //O(1)
        return true;
    }
}



bool Datastructures::dequrecu(std::shared_ptr<Region> reg,std::deque<RegionID>& path){
    path.push_front(reg->rid_);
    if(!reg->parent_){return false;}
    else{dequrecu(reg->parent_,path);}
    return true;
}


//PASKA FUNKTIO
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    std::deque<RegionID> path1 = {};
    std::deque<RegionID> path2 = {};
    dequrecu(Regions[id1],path1);   //O(n)
    dequrecu(Regions[id2],path2);   //O(n)
    int range = (path1.size()>path2.size()) ? path1.size() : path2.size();
    for(int i=0;i<range;++i){   //O(n)
        if(path1.at(i)==path2.at(i)){
            return path1.at(i);
        }
    }
    return NO_REGION;
}
