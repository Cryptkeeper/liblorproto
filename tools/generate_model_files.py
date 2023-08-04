#!/usr/local/bin/python3
import re
import urllib.request


class HardwareModel:
    def __init__(self, _pid, _name, _version):
        self.pid = _pid
        self.name = _name
        self.version = _version

    def get_pid(self):
        return self.pid

    def get_name(self):
        if self.version is not None:
            return f"{self.name} v{self.parse_version()}"
        return self.name

    def parse_version(self):
        if self.version is not None:
            # only use the major version (minor version is always 0|?)
            return self.version.split('.')[0]
        else:
            return None

    def get_id(self):
        id = self.name.upper().replace('-', '_').replace(' ', '_').replace('/', '__')
        return f"LOR_MODEL_{id}"

    def define_str(self):
        id = self.get_id()
        if self.version is not None:
            id += f"_V{self.parse_version()}"

        # include a unique identifier in the form of its hex value
        # LOR_DeviceFile.txt may contain the same model information across multiple lines with the same pid
        # which would otherwise create a naming collision
        id += f"_{self.pid:X}"

        return f"#define {id:36} ((LorModel){str(self.pid):>3}) /* {hex(self.pid)} */"

    def table_init_str(self, lowestPid):
        return f"\t[{self.pid - lowestPid}] = \"{self.name}\","


def request_devfile():
    req = urllib.request.Request("http://www1.lightorama.com/downloads/LOR_DeviceFile.txt")

    # required, otherwise server rejects with a 403 Forbidden
    req.add_header("user-agent", "curl/7.79.1")

    resp = urllib.request.urlopen(req)
    devf = resp.read().decode('utf-8')
    return devf


devfile = request_devfile()

# write the devfile to a local copy
# this is currently unused, but nice to have in the git history
f = open("LOR_DeviceFile.txt", "w")
f.write(devfile)
f.write("\n")  # ensure file always ends with newline
f.close()


def parse_models(devfile):
    models = []

    for line in devfile.split("\n"):
        result = re.search(r"^\s*(\d+),([\w-]+)\s+(Ver (\d\.[\d?]))?", line)

        if result is not None:
            pid, name, _, version = result.groups()
            models.append(HardwareModel(int(pid), name, version))

    return models


def deduplicate_models(models):
    pid_table = {}

    for model in models:
        if model.pid in pid_table:
            pid_table[model.pid].append(model)
        else:
            pid_table[model.pid] = [model]

    flat_models = []

    for pid in pid_table.keys():
        model_count = len(pid_table[pid])
        if model_count > 1:
            print(f"{model_count} overlapping models with pid: {pid}")

            # merge the model names together
            # version is set to None, but included in formatted #get_name
            new_name = "/".join(list(map(HardwareModel.get_name, pid_table[pid])))

            flat_models.append(HardwareModel(pid, new_name, None))
        else:
            flat_models.append(pid_table[pid][0])

    return flat_models


def get_model_id_range(models):
    pids = list(map(HardwareModel.get_pid, models))
    return min(pids), max(pids)


# method to keep the same builder pattern throughout the script
def get_auto_generated_warning():
    return """// AUTOMATICALLY GENERATED FILE, DO NOT MODIFY!
// YOUR CHANGES WILL BE OVERWRITTEN BY SCRIPT:
// liblightorama/tools/generate_model_files.py"""


models = deduplicate_models(parse_models(devfile))
lowestPid, highestPid = get_model_id_range(models)

print(f"parsed {len(models)} models (id range {lowestPid}-{highestPid})")


def fmt_model_h():
    models_str = "\n".join(list(map(HardwareModel.define_str, models)))
    return f"""{get_auto_generated_warning()}
#ifndef LIGHTORAMA_MODEL_H
#define LIGHTORAMA_MODEL_H

#include <stdbool.h>

typedef int LorModel;

{models_str}

bool lorGetModelName(LorModel model, const char **name);

#define lorGetMaxModel() ((LorModel){highestPid})

#endif //LIGHTORAMA_MODEL_H
"""


f = open("../include/lightorama/model.h", "w")
f.write(fmt_model_h())
f.close()


def fmt_model_c():
    inits = []
    for model in models:
        inits.append(model.table_init_str(lowestPid))

    table_init_str = "\n".join(inits)
    get_model_name_str = f"""static const char *LOR_MODEL_NAMES[{highestPid - lowestPid + 1}] = {{
    {table_init_str}
}};

bool lorGetModelName(const LorModel model, const char **const name) {{
    if (model < {lowestPid} || model > {highestPid}) {{
        return false;
    }}
    const char *modelName = LOR_MODEL_NAMES[(int)(model - {lowestPid})];
    if (!modelName) {{
        return false;
    }}
    *name = modelName;
    return true;
}}"""

    return f"""{get_auto_generated_warning()}
#include \"lightorama/model.h\"

{get_model_name_str}
"""


f = open("../src/model.c", "w")
f.write(fmt_model_c())
f.close()
