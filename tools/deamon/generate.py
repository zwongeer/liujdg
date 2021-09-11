#!/usr/bin/python3
import os
import re

os.chdir(os.path.dirname(os.path.abspath(__file__)))

all_apis_h = open("all_apis.hpp", "w")
apis_str = open("Lapi.hpp", "r").read()
all_apis_h.write("// This file is auto generated. Do not edit.\n")
all_apis_h.write(apis_str + "\n")

# to use all apis
call_all = ""

files = [f for f in os.listdir(".") if os.path.isfile(os.path.join(".", f))]
m = re.compile(r"L(.*?)\.cxx")
for f in files:
    if m.match(f):
        all_apis_h.write(f"void Lapi_{m.match(f).groups()[0]}(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config);\n")
        call_all += f"Lapi_{m.match(f).groups()[0]}(router, config);\n";


all_apis_h.write("\ninline void use_all_apis(std::unique_ptr<router_t>& router, const liujdg_deamon_config& config) {\n")
all_apis_h.write(call_all)
all_apis_h.write("}")