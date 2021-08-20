# Multiplication and Division Unit -MDU
This multiplication and division unit is compliance with RISC-V standard M-extension, which can be integreated with any existing RISC-V processor.

Two operands, function3, and a valid signal are fetched from the target processor and fed to the MDU. The function3 signal which is named as `i_mdu_op`, it decides that which operation MDU has to perform on the arrival of valid signal as shown in the table below.

| Instruction   |    i_mdu_op   |
|---------------|:-------------:|
| MUL           |      000      |
| MULH          |      001      |
| MULHSU        |      010      |
| MULHU         |      011      |
| DIV           |      100      |
| DIVU          |      101      |
| REM           |      110      |
| REMU          |      111      |

---
## Instantiation template
```
mdu_top
#(
  .WIDTH(32)
)i_mdu_top(
  .i_clk,  
  .i_rst,
  .i_mdu_rs1,
  .i_mdu_rs2,
  .i_mdu_op,
  .i_mdu_valid,
  .o_mdu_ready,
  .o_mdu_rd
);
```
---
## Getting Started
MDU uses [FuseSoC](https://github.com/olofk/fusesoc), which is a famous package manger and build abstraction tool for FPGA/ASIC development.

If the target core supports FuseSoC than you can easily add mdu as dependency in `.core` file and making changes in RTL.

### Demo: MDU integration with SERV
[SERV](https://github.com/olofk/serv) is the world smallest bit-serial RISC-V core. It is famous because of it's size and lumbering pace. The changes made for this integration can be tracked in [PR #60](https://github.com/olofk/serv/pull/60).  
SERV uses FuseSoC so it make our work more easier, you can run the SERV with MDU by simply following the steps below.

Create a directory to keep all the different parts of the project together. We will refer to this directory as `$WORKSPACE` from now on. All commands will be run from this directory unless otherwise stated.

Install FuseSoC

    pip install fusesoc 

Add SERV as a separate library into the workspace

    fusesoc library add serv https://github.com/olofk/serv

Now add MDU

    fusesoc library add mdu https://github.com/zeeshanrafique23/mdu

If [Verilator](https://www.veripool.org/wiki/verilator) is installed, we can use that as a linter to check the SERV source code

    fusesoc run --target=lint serv

If everything worked, the output should look like

    INFO: Preparing ::serv:1.1.0
    INFO: Setting up project

    INFO: Building simulation model
    INFO: Running

Now it's time to run SERV with MDU

    fusesoc run --target=verilator_tb --flag=mdu servant

The integration of MDU with SERV was the part of Google Summer of Code 2021 project under the banner of FOSSI Foundation, it is completed by [Zeeshan Rafique](https://github.com/zeeshanrafique23) under the mentorship of [Olof Kindgren](https://github.com/olofk).

Writing in progress...