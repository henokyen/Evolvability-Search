#ifndef _LINK_H_
#define _LINK_H_

#include "neat.h"
#include "trait.h"
#include "nnode.h"
#include <boost/serialization/binary_object.hpp>

namespace NEAT {

	class NNode;
	class Trait;

	// ----------------------------------------------------------------------- 
	// A LINK is a connection from one node to another with an associated weight 
	// It can be marked as recurrent 
	// Its parameters are made public for efficiency 
	class Link {
	public: 
		double weight; // Weight of connection
		NNode *in_node; // NNode inputting into the link
		NNode *out_node; // NNode that the link affects
		bool is_recurrent;
		bool time_delay;

		Trait *linktrait; // Points to a trait of parameters for genetic creation

		int trait_id;  // identify the trait derived by this link

		// ************ LEARNING PARAMETERS *********** 
		// These are link-related parameters that change during Hebbian type learning

		double added_weight;  // The amount of weight adjustment 
		std::vector <double> params;

		Link(double w,NNode *inode,NNode *onode,bool recur);

		// Including a trait pointer in the Link creation
		Link(Trait *lt,double w,NNode *inode,NNode *onode,bool recur);

		// For when you don't know the connections yet
		Link(double w);
		Link();
		// Copy Constructor
		Link(const Link& link);

		// Derive a trait into link params
		void derive_trait(Trait *curtrait);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(weight);
			ar & BOOST_SERIALIZATION_NVP(in_node);
			ar & BOOST_SERIALIZATION_NVP(out_node);
			ar & BOOST_SERIALIZATION_NVP(is_recurrent);
			ar & BOOST_SERIALIZATION_NVP(time_delay);
			ar & BOOST_SERIALIZATION_NVP(linktrait);
			ar & BOOST_SERIALIZATION_NVP(trait_id);
			ar & BOOST_SERIALIZATION_NVP(added_weight);
			//ar & boost::serialization::make_binary_object(params, 8);
			ar & BOOST_SERIALIZATION_NVP(params);

		}


	};

} // namespace NEAT

#endif
