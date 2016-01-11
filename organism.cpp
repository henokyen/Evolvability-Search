#include <cstring>
#include "organism.h"
#include "noveltyset.h"
//#include "boost/serialization/nvp.hpp"

using namespace NEAT;

Organism::Organism(double fit, Genome *g,int gen) {
	noveltypoint=NULL;
    datarec=NULL;
	fitness=fit;
	orig_fitness=fitness;
	gnome=g;
	net=gnome->genesis(gnome->genome_id);
	species=0;  //Start it in no Species
	expected_offspring=0;
	generation=gen;
	eliminate=false;
	error=0;
	off=false;
	winner=false;
	champion=false;
	super_champ_offspring=0;

	// If md is null, then we don't have metadata, otherwise we do have metadata so copy it over
/*	if(md == 0) {
		strcpy(metadata, "");
	} else {
		strncpy(metadata, md, 128);
	}*/

	time_alive=0;

	//DEBUG vars
	pop_champ=false;
	pop_champ_child=false;
	high_fit=0;
	mut_struct_baby=0;
	mate_baby=0;
	modified = true;
	blacklist =false;
	closest =0.0;
	closest_pt=NULL;
	BDO=0;
	FDO=0.0;
	PDO=0.0;

}
Organism::Organism()
{
	//std::cout<<"inside  organism default  constructor"<<std::endl;
	species =0;
	datarec=0;
	closest_pt=0;
	//noveltypoint=0;
}
Organism::Organism(const Organism& org,bool copy_data)
{
    off=org.off;
	BDO=org.BDO;
    FDO=org.FDO;
    PDO=org.PDO;
	datarec=NULL;
	noveltypoint=NULL;
	fitness = org.fitness;
        fit=org.fit;
        novelty=org.novelty;
	orig_fitness = org.orig_fitness;
	if(org.noveltypoint)
	{
		noveltypoint=new noveltyitem(*(org.noveltypoint));
	}
        if(copy_data) {
                datarec=new data_record(*(org.datarec));
        }

	gnome = new Genome(*(org.gnome));	// Associative relationship
	//gnome = org.gnome->duplicate(org.gnome->genome_id);
	net = new Network(*(org.net)); // Associative relationship
	species = org.species;	// Delegation relationship
	expected_offspring = org.expected_offspring;
	generation = org.generation;
	eliminate = org.eliminate;
	error = org.error;
	winner = org.winner;
	champion = org.champion;
	super_champ_offspring = org.super_champ_offspring;

	strcpy(metadata, org.metadata);
	//printf("copying %s did it work? %s", org.metadata, metadata);

	time_alive = org.time_alive;
	pop_champ = org.pop_champ;
	pop_champ_child = org.pop_champ_child;
	high_fit = org.high_fit;
	mut_struct_baby = org.mut_struct_baby;
	mate_baby = org.mate_baby;
closest_pt=org.closest_pt;
	modified = false;
}

Organism::~Organism() {
	delete net;
	delete gnome;
        if(datarec)
          delete datarec;
    if(noveltypoint)
	{
		if(!noveltypoint->added){
			//cout<<"Deleting novelty point"<<endl;
			delete noveltypoint;
		}
	}
    if(closest_pt)
    	delete closest_pt;
}
bool Organism::dominate(Organism* k)
{
 /*if (FDO < k->FDO || BDO < k->BDO)
    return false;
 if (FDO > k->FDO || BDO > k->BDO)
    return true;
 return false;*/

 if (((FDO > k->FDO) && (novelty >= k->novelty))
	      		   ||
    ((novelty > k->novelty) && (FDO >= k->FDO)))
	 return true;
  else
  return false;
}

void Organism::update_phenotype() {

	//First, delete the old phenotype (net)
	delete net;

	//Now, recreate the phenotype off the new genotype
	net=gnome->genesis(gnome->genome_id);

	modified = true;
}

bool Organism::print_to_file(char *filename) {
	
	std::ofstream oFile(filename);
    //Put the fitness for each organism in a comment
    oFile<<std::endl<<"/* Organism #"<<(this->gnome)->genome_id<<" Fitness: "<<this->fitness<<" Error: "<<this->error<<" */"<<std::endl;

    //If it is a winner, mark it in a comment
    //if (winner) oFile<<"/* ##------$ WINNER "<<(this->gnome)->genome_id<<" SPECIES #"<<id<<" $------## */"<<std::endl;

     write_to_file(oFile);

	if (this->noveltypoint)
	{
		this->noveltypoint->SerializeNoveltyPoint(oFile);
	}

	return 1;
}

bool Organism::write_to_file(std::ostream &outFile) {
	
	char tempbuf2[1024];
	if(modified == true) {
		sprintf(tempbuf2, "/* Organism #%d Fitness: %f Time: %d */\n", (gnome)->genome_id, orig_fitness, time_alive);
	} else {
		sprintf(tempbuf2, "/* %s */\n", metadata);
	}
	outFile << tempbuf2;

	gnome->print_to_file(outFile);
	return 1;
}

//// Print the Organism's genome to a file preceded by a comment             
////   detailing the organism's species, number, and fitness
//bool Organism::print_to_file(char *filename) {
//
//ofstream oFile(filename,ios::out);
//
//cout<<"FILENAME: "<<filename<<endl;
//
////Make sure it worked
//if (!oFile) {
//cerr<<"Can't open "<<filename<<" for output"<<endl;
//return 0;
//}
//
////Put the fitness and other information for each organism in a comment
//oFile<<endl<<"/* Organism #"<<gnome->genome_id<<" Fitness: "<<fitness<<" *///"<<endl;
//
//gnome->print_to_file(oFile);
//
//return 1;
//}
bool NEAT::order_orgs_NSGA(Organism *x, Organism *y)
{
	return (x)->fit > (y)->fit;
}
bool NEAT::order_orgs(Organism *x, Organism *y) {
	return (x)->fitness > (y)->fitness;
}

bool NEAT::order_orgs_by_adjusted_fit(Organism *x, Organism *y) {
	return (x)->fitness / (x->species)->organisms.size()  > (y)->fitness / (y->species)->organisms.size();
}
