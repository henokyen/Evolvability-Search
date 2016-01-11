#ifndef _NNODE_H_
#define _NNODE_H_

#include <algorithm>
#include <vector>
#include "neat.h"
#include "trait.h"
#include "link.h"
#include "boost/serialization/nvp.hpp"
#include <boost/serialization/binary_object.hpp>
namespace NEAT {
	class Network;
	enum nodetype {
		NEURON = 0,
		SENSOR = 1
	};

	enum nodeplace {
		HIDDEN = 0,
		INPUT = 1,
		OUTPUT = 2,
		BIAS = 3
	};

	enum functype {
		SIGMOID = 0
	};

	class Link;
	class Trait;

	// ----------------------------------------------------------------------- 
	// A NODE is either a NEURON or a SENSOR.  
	//   - If it's a sensor, it can be loaded with a value for output
	//   - If it's a neuron, it has a list of its incoming input signals (List<Link> is used) 
	// Use an activation count to avoid flushing
	class NNode {

		friend class Network;
		friend class Genome;

	protected:
		int activation_count;  // keeps track of which activation the node is currently in
		double last_activation; // Holds the previous step's activation for recurrency
		double last_activation2; // Holds the activation BEFORE the prevous step's
		// This is necessary for a special recurrent case when the innode
		// of a recurrent link is one time step ahead of the outnode.
		// The innode then needs to send from TWO time steps ago

		Trait *nodetrait; // Points to a trait of parameters

		int trait_id;  // identify the trait derived by this node

		NNode *dup;       // Used for Genome duplication

		NNode *analogue;  // Used for Gene decoding

		bool override; // The NNode cannot compute its own output- something is overriding it

		double override_value; // Contains the activation value that will override this node's activation

		// Pointer to the Sensor corresponding to this Body.
		//Sensor* mySensor;

	public:
		bool frozen; // When frozen, cannot be mutated (meaning its trait pointer is fixed)

		functype ftype; // type is either SIGMOID ..or others that can be added
		nodetype type; // type is either NEURON or SENSOR 
		
        //jladd
        double time_const;
        double bias;

		double activesum;  // The incoming activity before being processed 
		double activation; // The total activation entering the NNode 
		bool active_flag;  // To make sure outputs are active

		// NOT USED IN NEAT - covered by "activation" above
		double output;  // Output of the NNode- the value in the NNode 

		// ************ LEARNING PARAMETERS *********** 
		// The following parameters are for use in    
		//   neurons that learn through habituation,
		//   sensitization, or Hebbian-type processes  

		std::vector<double> params;//[NEAT::num_trait_params];

		std::vector<Link*> incoming; // A list of pointers to incoming weighted signals from other nodes
		std::vector<Link*> outgoing;  // A list of pointers to links carrying this node's signal

		// These members are used for graphing with GTK+/GDK
		std::vector<double> rowlevels;  // Depths from output where this node appears
		int row;  // Final row decided upon for drawing this NNode in
		int ypos;
		int xpos;

		int node_id;  // A node can be given an identification number for saving in files

		nodeplace gen_node_label;  // Used for genetic marking of nodes

		NNode(nodetype ntype,int nodeid);

		NNode(nodetype ntype,int nodeid, nodeplace placement);

		// Construct a NNode off another NNode for genome purposes
		NNode(NNode *n,Trait *t);

		// Construct the node out of a file specification using given list of traits
		NNode (const char *argline, std::vector<Trait*> &traits);

		// Copy Constructor
		NNode (const NNode& nnode);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(incoming);
			ar & BOOST_SERIALIZATION_NVP(outgoing);
			ar & BOOST_SERIALIZATION_NVP(nodetrait);
			ar & BOOST_SERIALIZATION_NVP(dup);
			ar & BOOST_SERIALIZATION_NVP(analogue);
			ar & BOOST_SERIALIZATION_NVP(activation_count);
			ar & BOOST_SERIALIZATION_NVP(last_activation);
			ar & BOOST_SERIALIZATION_NVP(last_activation2);
			ar & BOOST_SERIALIZATION_NVP(trait_id);
			ar & BOOST_SERIALIZATION_NVP(override);
			ar & BOOST_SERIALIZATION_NVP(override_value);
			ar & BOOST_SERIALIZATION_NVP(frozen);
			ar & BOOST_SERIALIZATION_NVP(ftype);
			ar & BOOST_SERIALIZATION_NVP(type);
			ar & BOOST_SERIALIZATION_NVP(time_const);
			ar & BOOST_SERIALIZATION_NVP(bias);
			ar & BOOST_SERIALIZATION_NVP(activesum);
			ar & BOOST_SERIALIZATION_NVP(activation);
			ar & BOOST_SERIALIZATION_NVP(active_flag);
			ar & BOOST_SERIALIZATION_NVP(output);
			//ar & boost::serialization::make_binary_object(params, 8);
			ar & BOOST_SERIALIZATION_NVP(params);
			ar & BOOST_SERIALIZATION_NVP(rowlevels);
			ar & BOOST_SERIALIZATION_NVP(row);
			ar & BOOST_SERIALIZATION_NVP(ypos);
			ar & BOOST_SERIALIZATION_NVP(xpos);
			ar & BOOST_SERIALIZATION_NVP(node_id);
			ar & BOOST_SERIALIZATION_NVP(gen_node_label);


		}

		NNode();
		~NNode();

		// Just return activation for step
		double get_active_out();
		double get_output();

		// Return activation from PREVIOUS time step
		double get_active_out_td();

		// Returns the type of the node, NEURON or SENSOR
		const nodetype get_type();

		// Allows alteration between NEURON and SENSOR.  Returns its argument
		nodetype set_type(nodetype);

		// If the node is a SENSOR, returns true and loads the value
		bool sensor_load(double);

		// Adds a NONRECURRENT Link to a new NNode in the incoming List
		void add_incoming(NNode*,double);

		// Adds a Link to a new NNode in the incoming List
		void add_incoming(NNode*,double,bool);

		// Recursively deactivate backwards through the network
		void flushback();

		// Verify flushing for debugging
		void flushback_check(std::vector<NNode*> &seenlist);

		// Print the node to a file
        void  print_to_file(std::ostream &outFile);
	void print_to_file(std::ofstream &outFile);

		// Have NNode gain its properties from the trait
		void derive_trait(Trait *curtrait);

		// Returns the gene that created the node
		NNode *get_analogue();

		// Force an output value on the node
		void override_output(double new_output);

		// Tell whether node has been overridden
		bool overridden();

		// Set activation to the override value and turn off override
		void activate_override();  

		// Writes back changes weight values into the genome
		// (Lamarckian trasnfer of characteristics)
		void Lamarck();

		//Find the greatest depth starting from this neuron at depth d
		int depth(int d,Network *mynet,int& count, int thresh); 
		
	};

	
} // namespace NEAT

#endif
