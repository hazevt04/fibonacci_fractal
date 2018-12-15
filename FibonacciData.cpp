#include <string>

class FibonacciData {
   public:
      FibonacciData( bool verbose_flag, unsigned int color, 
            unsigned long num_iterations, std::string output_file ) {
      
         this->verbose_flag = verbose_flag;
         this->color = color;
         this->num_iterations = num_iterations;
         this->output_file = output_file;
      }

      ~FibonacciData() { }
   private:
      bool verbose_flag;
      unsigned int color;
      unsigned long num_iterations;
      std::string output_file;
};
