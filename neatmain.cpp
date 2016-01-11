#include <iostream>
#include <fstream>
using namespace std;
#include <tclap/CmdLine.h>
#include <cstring>
#include <vector>
#include <unistd.h>	
#include "neat.h"
#include "population.h"
#include "experiments.h"
#include "biped.h"

using namespace TCLAP;
static bool biped=false;

int main(int argc, char **argv) {
	//class Organism;

    //sferes::eval::Mpi* mp = new sferes::eval::Mpi();
	//sferes::eval::Mpi<Organism*>*
	//evall= new sferes::eval::Mpi<Organism*>();
	//NEAT::eval_t  evall;
/*	if (evall==0)
	{}
   // eval_t eval;
	else*/
	//NEAT::eval_t evall;
     //cout<<"How many processes :"<<evall._world->size()<<endl;
  if (evall._world->rank()==0)
	{
	  //cout<<"How many processors :"<<evall._world->size()<<endl;

/*    std::vector<int> test;
    test.push_back(12);
    test.push_back(52);
    test.push_back(19);
    test.push_back(45);
    test.push_back(87);
    test.push_back(20);
    test.push_back(30);
    test.push_back(40);
    vector<Organism> pop2;
    for (int j =0;j<pop->organisms.size();j++){
    	pop2.push_back(*pop->organisms[j]);
    	//cout<<"my fitness: "<<pop2[j].fitness<<endl;
    }*/
    //eval.eval(pop->organisms,0,pop->organisms.size());
   // eval.eval(pop2,0,pop2.size());


  CmdLine cmd("Maze evolution", ' ', "0.1");
   
  ValueArg<string> maze("m","maze","Maze file",false,"medium_maze_list.txt","string");
  cmd.add(maze);
  
  ValueArg<string> mcmaze("","mcmaze","MC Maze file",false,"mcmaze.txt","string");
  cmd.add(mcmaze);

  ValueArg<string> genes("z","sg","Starter genes",false,"mazestart_orig","string");
  cmd.add(genes);

  SwitchArg EvoSwitch("e","evo","Enable Evo search",false);
  cmd.add(EvoSwitch);

  SwitchArg MultiSwitch("t","EPEB","Enable EP&EB multiobjective search",false);
  cmd.add(MultiSwitch);

  ValueArg<string> settings("s","settings","Settings file",false,"maze.ne","string");
  cmd.add(settings);
 
  ValueArg<string> output("o","output","Output directory",false,"./results","string");
  cmd.add(output);

  ValueArg<string> seed_genome("c","seed","Seed Genome",false,"","string");
  cmd.add(seed_genome);
 
  SwitchArg local_switch("","lc","Local competition",false);
  cmd.add(local_switch);

  SwitchArg biped_switch("","biped","Biped domain",false);
  cmd.add(biped_switch);
 
  SwitchArg remove_random("","remrand","Remove random individuak",false);
  cmd.add(remove_random); 

  SwitchArg extinction("","extinct","Turn on random extinctions",false);
  cmd.add(extinction);

  SwitchArg goal_attract("","goalnotsticky","Goal is not attractor",false);
  cmd.add(goal_attract);

  SwitchArg area_of_interest("","aoi","Enforce pruning of behavior space",false);
  cmd.add(area_of_interest);

  SwitchArg noveltySwitch("n","novelty","Enable novelty search",false);
  cmd.add(noveltySwitch);

  SwitchArg evaluateSwitch("","eval","Evaluate a genome",false);
  cmd.add(evaluateSwitch);

  SwitchArg constraintSwitch("","constraint","Enable constraint-based NS",false);
  cmd.add(constraintSwitch);

  SwitchArg generationalSwitch("g","gen","Enable generational search",false);
  cmd.add(generationalSwitch);

  SwitchArg mcSwitch("","mc","Enable minimal criteria",false);
  cmd.add(mcSwitch);


  ValueArg<string> nov_measure("","nm","Novelty Measure",false,"std","string");
  cmd.add(nov_measure);

  ValueArg<string> fit_measure("f","fm","Fitness Measure",false,"goal","string");
  cmd.add(fit_measure);

  ValueArg<int> extra_param("p","parameter","Extra Parameter",false,0,"int");
  cmd.add(extra_param);

  ValueArg<int> num_samples("a","samples","Num Samples",false,1,"int");
  cmd.add(num_samples);

  ValueArg<int> time_steps("b","timesteps","Num Timesteps",false,400,"int");
  cmd.add(time_steps);

  ValueArg<int> rng_seed("r","random_seed","Random Seed",false,-1,"int");
  cmd.add(rng_seed);
  //cout<<"printting the contents of cmd \n";
  //cmd.print_arglist();
  cmd.parse(argc,argv);
  //cout<<"printting the contents of cmd after parsing\n";
  //cout<<"#########################\n";
  //cmd.print_arglist();

  char mazename[100]="hard_maze_list.txt";
  char filename[100]="./runoutput_";
  char settingsname[100]="maze.ne";
  char startgenes[100]="mazestartgenes";
  int param;
  NEAT::Population *p;
  //cout<<"this is the filename" <<filename<<"\n";
  //***********RANDOM SETUP***************//
  /* Seed the random-number generator with current time so that
      the numbers will be different every time we run.    */
  srand( (unsigned)time( NULL )  + getpid());
 
  if(rng_seed.getValue()!=-1)
	srand((unsigned)rng_seed.getValue());

  strcpy(settingsname,settings.getValue().c_str());
  strcpy(mazename,maze.getValue().c_str());
  strcpy(filename,output.getValue().c_str());
  strcpy(startgenes,genes.getValue().c_str());
 //cout<<"this is the filename" <<filename<<"\n";
  NEAT::load_neat_params(settingsname,true);
  
  if(local_switch.getValue()) 
   NEAT::local_competition=true;

  param = extra_param.getValue();
  cout<<"loaded"<<endl;

  //cout << "Maze: " << mazename << endl;
  cout << "Start genes: " << startgenes << endl;
  
  set_evaluate(evaluateSwitch.getValue());
  set_extinction(extinction.getValue());
  set_mcmaze(mcmaze.getValue());
  set_fit_measure(fit_measure.getValue());
  set_nov_measure(nov_measure.getValue());
  set_aoi(area_of_interest.getValue());
   set_random_replace(remove_random.getValue());
  cout << "Timesteps: " << time_steps.getValue() << endl;
  set_timesteps(time_steps.getValue());

  cout << "Num Samples: " << num_samples.getValue() << endl;
  set_samples(num_samples.getValue());

  set_seed(seed_genome.getValue()); 

   cout << "Goal not sticky? "  << goal_attract.getValue() << endl;
  set_goal_attract(!goal_attract.getValue());

  cout << "Minimal criteria engaged? " << mcSwitch.getValue() << endl;
  set_minimal_criteria(mcSwitch.getValue());

if(biped_switch.getValue())
	biped=true;
 set_constraint_switch(constraintSwitch.getValue());

 if(!generationalSwitch.getValue())
 {
  if(!biped)
  {
	if(!noveltySwitch.getValue() && !EvoSwitch.getValue())
	{
	  cout <<"Fitness search enabled \n";
	  p = maze_novelty_realtime(filename,mazename,param,startgenes,false, false);
	}
	else if (!noveltySwitch.getValue())
	{
	  cout <<"Evo search enabled \n";
	  p = maze_novelty_realtime(filename,mazename,param,startgenes,false, true);
	}
	else
	{
		  cout <<"Novelty search enabled \n";
		  p = maze_novelty_realtime(filename,mazename,param,startgenes,true, false);
	}
 }
  else
  {
  //cout<<"Are in here \n";	  
  if(!noveltySwitch.getValue() && !EvoSwitch.getValue())
  {
	  cout <<"Fitness search enable for Biped \n";
	  p = biped_novelty_realtime(filename,mazename,param,startgenes,false,false);
  }
else if (!noveltySwitch.getValue() && EvoSwitch.getValue())
	{
	  cout <<"Evo search enabled for Biped\n";
	  p = biped_novelty_realtime(filename,mazename,param,startgenes,false, true);
	}
  else
  {
	  cout <<"Novelty search enabled for Biped\n";
	  p = biped_novelty_realtime(filename,mazename,param,startgenes,true, false);
  }
 }
}
else
{
 cout<<"you here right?"<<endl;
 if(!biped){ 
	 if (!MultiSwitch.getValue())
		{
			cout<<"Generational novelty search Enabled!! \n";
			p = maze_generational(filename,mazename,param,startgenes,1000,noveltySwitch.getValue());
		}

		else if (MultiSwitch.getValue())
		{
			cout<<"Multi-objective generational search with EP&EB is Enabled!! \n";
			p = maze_generational(filename,mazename,param,startgenes,1000,noveltySwitch.getValue());
		}
}
 else
{
 cout<<"Multi-objective on Biped domain";
 p = biped_generational(filename,startgenes,1000,noveltySwitch.getValue());
 }

}

}
else{
     std::cout << "I am process " << evall._world->rank();
     evall._slave_loop<NEAT::Organism*>();
	 //evall._world->barrier();
}  /*  else
    	evall->_finalize();*/
return 0;
}
