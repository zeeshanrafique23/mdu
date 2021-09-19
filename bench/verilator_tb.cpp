#include <stdint.h>
#include <signal.h>
// #include <stdlib.h> 
// By including stdlib, we can use rand() to generate stimulus

#include "Vmdu_top.h"
#include "verilated_vcd_c.h"

using namespace std;

static bool done;

vluint64_t main_time = 0;      // Current simulation time
// This is a 64-bit integer to reduce wrap over issues and
// allow modulus.  You can also use a double, if you wish.

void INThandler(int signal) {
	printf("\nCaught ctrl-c\n");
	done = true;
}

int main(int argc, char** argv, char** env) {

  Verilated::commandArgs(argc, argv);
  Vmdu_top* top = new Vmdu_top;

  VerilatedVcdC * tfp = 0;
	const char *vcd = Verilated::commandArgsPlusMatch("vcd=");
	if (vcd[0]) {
	  Verilated::traceEverOn(true);
	  tfp = new VerilatedVcdC;
	  top->trace (tfp, 99);
	  tfp->open ("trace.vcd");
	}

  signal(SIGINT, INThandler);
  
  vluint64_t timeout = 0;
	const char *arg_timeout = Verilated::commandArgsPlusMatch("timeout=");
	if (arg_timeout[0]) {
	  timeout = atoi(arg_timeout+9);
  }

	vluint64_t vcd_start = 0;
	const char *arg_vcd_start = Verilated::commandArgsPlusMatch("vcd_start=");
	if (arg_vcd_start[0]) {
	  vcd_start = atoi(arg_vcd_start+11);
  }

  bool dump = false;
  unsigned int mdu_ready;
  top->i_clk = 1;
  top->i_mdu_valid = 0;

  while (!(done || Verilated::gotFinish())) {
    if (tfp && !dump && (main_time > vcd_start)) {
	    dump = true;
	  }

	  top->i_rst = main_time < 50;
    if (!top->i_rst) {
      top->i_mdu_rs1 =  10;
      top->i_mdu_rs2 =  10;
      top->i_mdu_op  =   0; // Mulplication for now 10*10 = 100
                            // It would we great if someone could generate better stimulus
      top->i_mdu_valid = 1;
    }  
	  top->eval();

	  if (dump) {
	    tfp->dump(main_time);
    }

    if (timeout && (main_time >= timeout)) {
	    printf("Timeout: Exiting at time %lu\n", main_time);
	    done = true;
	  }
    top->i_clk = !top->i_clk;
    main_time += 10;
  }
  
  if (tfp)
	  tfp->close();
	exit(0);
}