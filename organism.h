#ifndef _ORGANISM_H_
#define _ORGANISM_H_

#include "genome.h"
#include "species.h"
#include "datarec.h"
#include "boost/serialization/nvp.hpp"
#include <boost/serialization/binary_object.hpp>

class noveltyitem;
	
namespace NEAT {

	class Species;
	class Population;
	

	// ---------------------------------------------  
	// ORGANISM CLASS:
	//   Organisms are Genomes and Networks with fitness
	//   information 
	//   i.e. The genotype and phenotype together
	// ---------------------------------------------  
	class Organism {

	public:
		bool off; //when true it indicates that the organism is a child
		int BDO;
		double FDO;
		double PDO;
		double fitness;  //A measure of fitness for the Organism
		double orig_fitness;  //A fitness measure that won't change during adjustments
		double error;  //Used just for reporting purposes
		bool winner;  //Win marker (if needed for a particular task)
                double fit;// A measure of rank in multi-objective search method NS+BDO/FDO/PDO
                double novelty;// A measure of novelty in multi-objective search method NS+BDO/FDO/PDO
		Network *net;  //The Organism's phenotype
		Genome *gnome; //The Organism's genotype 
		Species *species;  //The Organism's Species 
		noveltyitem *noveltypoint; //The Organism's Novelty Point
        data_record *datarec;
		double expected_offspring; //Number of children this Organism may have
		int generation;  //Tells which generation this Organism is from
		bool eliminate;  //Marker for destruction of inferior Organisms
		bool champion; //Marks the species champ
		int super_champ_offspring;  //Number of reserved offspring for a population leader
		bool pop_champ;  //Marks the best in population
		bool pop_champ_child; //Marks the duplicate child of a champion (for tracking purposes)
		double high_fit; //DEBUG variable- high fitness of champ
		int time_alive; //When playing in real-time allows knowing the maturity of an individual

		// Track its origin- for debugging or analysis- we can tell how the organism was born
		bool mut_struct_baby;
		bool mate_baby;

		//added for generational ns
		bool blacklist;
		double closest;
		noveltyitem* closest_pt;

		// MetaData for the object
		char metadata[128];
		bool modified;

		// Regenerate the network based on a change in the genotype 
		void update_phenotype();
		bool dominate(Organism* k);

		// Print the Organism's genome to a file preceded by a comment detailing the organism's species, number, and fitness 
		bool print_to_file(char *filename);   
		bool write_to_file(std::ostream &outFile);
		template<class Archive>
		void serialize(Archive & ar,  unsigned int version)
		{
			//boost::serialization::make_nvp(BOOST_PP_STRINGIZE(fitness), fitness);
		  ar & BOOST_SERIALIZATION_NVP(gnome);
		  ar & BOOST_SERIALIZATION_NVP(net);
		  //ar & BOOST_SERIALIZATION_NVP(species);
		  ar & BOOST_SERIALIZATION_NVP(noveltypoint);
		  //ar & BOOST_SERIALIZATION_NVP(datarec);
		  //ar & BOOST_SERIALIZATION_NVP(closest_pt);
		  ar & BOOST_SERIALIZATION_NVP(off);
		  //ar & BOOST_SERIALIZATION_NVP(EP);
		  //ar & BOOST_SERIALIZATION_NVP(EB);
		  //ar & BOOST_SERIALIZATION_NVP(orig_fitness);
		  ar & BOOST_SERIALIZATION_NVP(fitness);
		  //ar & BOOST_SERIALIZATION_NVP(error);
		  //ar & BOOST_SERIALIZATION_NVP(winner);
		  //ar & BOOST_SERIALIZATION_NVP(expected_offspring);
		  //ar & BOOST_SERIALIZATION_NVP(generation);
		  //ar & BOOST_SERIALIZATION_NVP(eliminate);
		  //ar & BOOST_SERIALIZATION_NVP(champion);
		  //ar & BOOST_SERIALIZATION_NVP(super_champ_offspring);
		  //ar & BOOST_SERIALIZATION_NVP(pop_champ);
		  //ar & BOOST_SERIALIZATION_NVP(pop_champ_child);
		  //ar & BOOST_SERIALIZATION_NVP(high_fit);
		  //ar & BOOST_SERIALIZATION_NVP(time_alive);
		  //ar & BOOST_SERIALIZATION_NVP(mut_struct_baby);
		  //ar & BOOST_SERIALIZATION_NVP(mate_baby);
		  //ar & BOOST_SERIALIZATION_NVP(blacklist);
		  //ar & BOOST_SERIALIZATION_NVP(closest);
		  //ar & boost::serialization::make_binary_object(metadata, 128);
		  //ar & BOOST_SERIALIZATION_NVP(metadata);
		  //ar & BOOST_SERIALIZATION_NVP(modified);

		 }
		Organism();
		Organism(double fit, Genome *g, int gen);
		Organism(const Organism& org,bool copy_data=false);	// Copy Constructor
		~Organism();

	};

	// This is used for list sorting of Organisms by fitness..highest fitness first
	bool order_orgs(Organism *x, Organism *y);
        bool order_orgs_NSGA(Organism *x, Organism *y);
	bool order_orgs_by_adjusted_fit(Organism *x, Organism *y);

} // namespace NEAT

#endif
