#ifndef _GENE_H_
#define _GENE_H_

#include "neat.h"
#include "trait.h"
#include "link.h"
#include "network.h"
#include "boost/serialization/nvp.hpp"

namespace NEAT {
class Link;
class Trait;

	class Gene {
	public:

		Link *lnk;
		double innovation_num;
		double mutation_num;  //Used to see how much mutation has changed the link
		bool enable;  //When this is off the Gene is disabled
		bool frozen;  //When frozen, the linkweight cannot be mutated

		//Construct a gene with no trait
		Gene(double w,NNode *inode,NNode *onode,bool recur,double innov,double mnum);

		//Construct a gene with a trait
		Gene(Trait *tp,double w,NNode *inode,NNode *onode,bool recur,double innov,double mnum);

		//Construct a gene off of another gene as a duplicate
		Gene(Gene *g,Trait *tp,NNode *inode,NNode *onode);

		//Construct a gene from a file spec given traits and nodes
		Gene(const char *argline, std::vector<Trait*> &traits, std::vector<NNode*> &nodes);

		// Copy Constructor
		Gene(const Gene& gene);
		template<class Archive>
		void Gene::serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(lnk);
			ar & BOOST_SERIALIZATION_NVP(innovation_num);
			ar & BOOST_SERIALIZATION_NVP(mutation_num);
			ar & BOOST_SERIALIZATION_NVP(enable);
			ar & BOOST_SERIALIZATION_NVP(frozen);
		}
		Gene();
		
		~Gene();

		//Print gene to a file- called from Genome
        void print_to_file(std::ostream &outFile);
	void print_to_file(std::ofstream &outFile);
	};

} // namespace NEAT


#endif
