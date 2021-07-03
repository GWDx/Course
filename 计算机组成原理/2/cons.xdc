set_property -dict { PACKAGE_PIN E3 	IOSTANDARD LVCMOS33 } [get_ports {clk}];
set_property -dict { PACKAGE_PIN B18	IOSTANDARD LVCMOS33 } [get_ports {button}];
create_clock -add -name sys_clk_pin -period 10.00 -waveform {0 5} [get_ports { clk }];


set_property -dict { PACKAGE_PIN D14   IOSTANDARD LVCMOS33 } [get_ports {sw[0]}];
set_property -dict { PACKAGE_PIN F16   IOSTANDARD LVCMOS33 } [get_ports {sw[1]}];
set_property -dict { PACKAGE_PIN G16   IOSTANDARD LVCMOS33 } [get_ports {sw[2]}];
set_property -dict { PACKAGE_PIN H14   IOSTANDARD LVCMOS33 } [get_ports {sw[3]}];
set_property -dict { PACKAGE_PIN E16   IOSTANDARD LVCMOS33 } [get_ports {sw[4]}];
set_property -dict { PACKAGE_PIN F13   IOSTANDARD LVCMOS33 } [get_ports {sw[5]}];
set_property -dict { PACKAGE_PIN G13   IOSTANDARD LVCMOS33 } [get_ports {sw[6]}];
set_property -dict { PACKAGE_PIN H16   IOSTANDARD LVCMOS33 } [get_ports {sw[7]}];


set_property -dict { PACKAGE_PIN A14   IOSTANDARD LVCMOS33 } [get_ports {seg[0]}];
set_property -dict { PACKAGE_PIN A13   IOSTANDARD LVCMOS33 } [get_ports {seg[1]}];
set_property -dict { PACKAGE_PIN A16   IOSTANDARD LVCMOS33 } [get_ports {seg[2]}];
set_property -dict { PACKAGE_PIN A15   IOSTANDARD LVCMOS33 } [get_ports {seg[3]}];
set_property -dict { PACKAGE_PIN B17   IOSTANDARD LVCMOS33 } [get_ports {an[0]}];
set_property -dict { PACKAGE_PIN B16   IOSTANDARD LVCMOS33 } [get_ports {an[1]}];
set_property -dict { PACKAGE_PIN A18   IOSTANDARD LVCMOS33 } [get_ports {an[2]}];


set_property -dict { PACKAGE_PIN C17	IOSTANDARD LVCMOS33 } [get_ports {led[0]}];
set_property -dict { PACKAGE_PIN D18	IOSTANDARD LVCMOS33 } [get_ports {led[1]}];
set_property -dict { PACKAGE_PIN E18	IOSTANDARD LVCMOS33 } [get_ports {led[2]}];
set_property -dict { PACKAGE_PIN G17	IOSTANDARD LVCMOS33 } [get_ports {led[3]}];
set_property -dict { PACKAGE_PIN D17	IOSTANDARD LVCMOS33 } [get_ports {led[4]}];
set_property -dict { PACKAGE_PIN E17	IOSTANDARD LVCMOS33 } [get_ports {led[5]}];
set_property -dict { PACKAGE_PIN F18	IOSTANDARD LVCMOS33 } [get_ports {led[6]}];
set_property -dict { PACKAGE_PIN G18	IOSTANDARD LVCMOS33 } [get_ports {led[7]}];


set_property CFGBVS VCCO [current_design];
set_property CONFIG_VOLTAGE 3.3 [current_design];
