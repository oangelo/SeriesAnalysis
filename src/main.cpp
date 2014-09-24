#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "time_series.h"
#include "attractor.h"
#include "rqa.h"
#include "pstatistics/statistics.h"
#include "boost/program_options.hpp" 

#define TRUE 1
#define FALSE 0


int main(int argc, char** argv) 
{ 

  std::vector<std::vector<double>> data;

  TimeSeries *time_series(nullptr);
  Attractor *attractor(nullptr);
  RecurrencePlot *rp(nullptr);

  unsigned rows(0),column(0), bins(0), dimension(0), delay(0), window;
  double threshold;
  std::string file_name;

  try 
  { 
    /** Define and parse the program options */ 
    namespace po = boost::program_options; 
    po::options_description basic("Main"); 
    basic.add_options() 
      ("help,h", "Print help messages") 
      ("file,f", po::value<std::string>(&file_name),"Read data from file" )
      ("stdin","Read data from the standard input " );

    po::options_description objects("Objects"); 
    objects.add_options()
      ("attractor","Creates an attractor from the input data or from a time \
series when both the specified dimension and delay are set.")
      ("dim,d", po::value<unsigned>(&dimension), "Dimension of embedding.")
      ("delay", po::value<unsigned>(&delay),"Time delay for embedding.")
      ("ts","Create a time series from the input data and column.")
      ("column,c", po::value<unsigned>(&column)->default_value(0),"The column from the file to use as time series.");

    po::options_description opt("Options"); 
    opt.add_options()
      ("threshold", po::value<double>(&threshold),"Threshold value for RPs.")
      ("bins", po::value<unsigned>(&bins),"Number of bins for mutual informations.")
      ("rp_std","Use as threshold a <real> * st. deviation of the time series.")
      ("window", po::value<unsigned>(&window),"Thrailer window / moving average.")
      ("surrogate","Shuffle the time series.");

      po::options_description methods("Methods"); 
      methods.add_options()
      ("fnn","False nearest Neighbors, a method to find embedding dimension.")
      ("mi","Mutual information, a method to find the embedding delay.")
      ("autocorrelation","Autocorrelation, another method to find the embedding delay.")
      ("moving_average","Calculate de time series moving average.")
      ("rp", "Print the Recurrence Plot from the input attractor.")
      ("rqa","Print the recurrence quantification analysis of the RP.");

    po::options_description all_options;
    all_options.add(basic).add(opt).add(methods).add(objects);
    po::variables_map vm; 
    try 
    { 
      po::store(po::command_line_parser(argc, argv).options(all_options).
          style( po::command_line_style::unix_style | po::command_line_style::allow_long_disguise).run(),
          vm); // can throw 
      if ( vm.count("help") or argc == 1) 
      { 
        std::cout << "Basic Command Line Parameter" << std::endl << basic << std::endl; 
        std::cout << objects << std::endl;
        std::cout << methods << std::endl;
        std::cout << opt << std::endl;
        return EXIT_SUCCESS; 
      } 

      po::notify(vm); // throws on error, so do after help in case 
      // there are any problems 
    } 
    catch(po::error& e) 
    { 
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
      return EXIT_FAILURE; 
    } 
    if (vm.count("stdin")){
      data = ReadStdin<double>();
      std::cerr << "Data size: " << data.size() << ", columns: " << data[0].size()  << std::endl;
    } else if (vm.count("file")){
      std::cerr << "Reading file: " << file_name  << std::endl;
      data =  ReadFile<double>(file_name);
      std::cerr << "Data size: " << data.size() << ", columns: " << data[0].size()  << std::endl;
    } else{
      throw std::invalid_argument("No input specified!");
    }

    //if the data is set, the we can create the objects
    if(data.size() > 0){
      if (vm.count("ts")){
        std::cerr << "Creating time series from file data" << std::endl;
        time_series = new  TimeSeries(data, column);
        if(vm.count("surrogate")){
            TimeSeries aux(Surrogate(*time_series));
            delete time_series;
            time_series = new TimeSeries(aux);
            std::cerr << "Shuffling time series" << std::endl;
        }

      }else if (vm.count("attractor") &&  !vm.count("dim") && !vm.count("delay")){
        std::cerr << "Creating attractor from file data" << std::endl;
        attractor = new Attractor(data);
      }else if (vm.count("attractor") &&  vm.count("dim") && vm.count("delay")){
        if(!time_series){
          time_series = new  TimeSeries(data, column);
          if(vm.count("surrogate")){
            TimeSeries aux(Surrogate(*time_series));
            delete time_series;
            time_series = new TimeSeries(aux);
            std::cerr << "Shuffling time series" << std::endl;
          }
        }
        std::cerr << "Creating attractor from time series" << std::endl;
        attractor = new Attractor(*time_series, dimension, delay);
      }else if(vm.count("dim") != vm.count("delay")){
        throw std::invalid_argument("Define both the dimension and the delay \
to create an attractor from a time series!");
      }else{
        throw std::invalid_argument("Create an object to be used on the analysis.");
      }
    }

    if (vm.count("rp") or vm.count("rqa")){
      if(!attractor)
        throw std::invalid_argument("No attractor to build a Recurrence Plot.");
      if (!vm.count("threshold")){ 
          std::cerr << "Trying to find a threshold for RP where recurrence has 5% level." << std::endl;
          threshold = FindThreshold(*attractor, 5, 0.1);
      } else if (vm.count("rp_std")) {
        std::cerr << "Using <threshold>*std as threshold for RP." << std::endl;
        threshold = threshold * StdPointsDistances(*attractor);
      }
      rp = new RecurrencePlot(*attractor, threshold);
      if(vm.count("window")){
        std::cerr << "Removing the Thrailer window." << std::endl;
        for (size_t j(0); j < window; ++j)
          for (size_t i(0); i < rp->size()-j; ++i)
            (*rp)[i][i+j]=0;
        for (size_t j(0); j < window; ++j)
          for (size_t i(j); i < rp->size(); ++i)
            (*rp)[i][i-j]=0;
      }
      if(vm.count("rqa")){
        RecurrenceAnalytics analytics(*rp);
        std::cout << analytics.RR()  << " "; 
        std::cout << analytics.DET()  << " "; 
        std::cout << analytics.L()  << " "; 
        std::cout << analytics.LMax()  << " "; 
        std::cout << " "; 
        std::cout << analytics.LAM()  << " "; 
        std::cout << analytics.TT()  << " "; 
        std::cout << analytics.VMax()  << " "; 
        std::cout << " "; 
        std::cout << analytics.RATIO()  << " "; 
        std::cout << analytics.NumberOfDiagonals()  << " "; 
        std::cout << analytics.NumberOfRecurrence()  << " "; 
        std::cout << std::endl; 
      }else{
        rp->PrintOnScreen();
      }
    } else if ("mi"){
      //if bins are not set, use a simple rule
      if(!vm.count("bins"))
        bins = sqrt(time_series->size());
      double normalize = MutualInformation(*time_series, 0, bins); 
      std::cout << "#delay x mutual_info" << std::endl;
      for (size_t i = 0; i < 0.01 * time_series->size(); ++i)
        std::cout << i << " " << MutualInformation(*time_series, i, bins) / normalize << std::endl; 
    } else if(vm.count("fnn")){
      std::cerr << ">> Nearest Neighbors Max Dimension: " << dimension  << std::endl; 
      std::cerr << ">> Delay: " << delay  << std::endl; 
      std::vector<unsigned> nff(FalseNearestNeighbors(*time_series, delay, dimension));

      std::cout << "# dimension x %FNN" << std::endl; 
      for (size_t i = 0; i < nff.size(); ++i)
        std::cout << i + 1 << " " << nff[i] * 100 / nff[0] << std::endl; 
    }else if(vm.count("autocorrelation")){
      std::cout << "# delay x auto_correlation" << std::endl;
      for (size_t i = 1; i < time_series->size() / 2.0; ++i)
        std::cout << i << " " << AutoCorrelation(*time_series, i) << std::endl; 
    } else if(vm.count("moving_average")){
      std::cerr << ">> Moving Average" << std::endl;
      MovingAverage(*time_series, window);
    }

    if(time_series)
      delete time_series;
    if(attractor)
      delete attractor;
    if(rp)
      delete rp;
  } 
  catch(std::exception& e) 
  { 
    std::cerr << "Unhandled Exception: " << e.what() <<  std::endl;
    std::cerr << "Application will now exit." << std::endl; 

    if(time_series)
      delete time_series;
    if(attractor)
      delete attractor;
    if(rp)
      delete rp;

    return EXIT_FAILURE; 

  } 
  return EXIT_SUCCESS; 
}
