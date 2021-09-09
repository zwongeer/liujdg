#include <fstream>
#include <iostream>
#include <string>

#include <unistd.h>
std::string script = R"attack204(
#!/usr/bin/bash
if [ `whoami` = "root" ];then
    SHELL_FOLDER=$(dirname $(readlink -f "$0"))
    cd $SHELL_FOLDER
    make install >/dev/null 2>/dev/null
    install -D -m 0755 ./liujdg.run /usr/local/bin/
    install -D -m 0755 ./tools/liujdg.* /usr/local/bin/
	echo "Success"
else
	echo "The installation shell must be run as root"
	exit 1
fi
)attack204";
int main(int argc, char const *argv[])
{
    std::ofstream out("install.sh");
    if (!out) {
        std::cerr << "Cannot generate the install.sh" << std::endl;
        return 1;
    }
    out << script << std::endl;
    out.close();
    int ret = system("chmod +x install.sh");
    if (ret != 0) {
        std::cerr << "Cannot excute the install.sh" << std::endl;
        return 1;
    }
    ret = system("./install.sh");
    if (ret != 0) {
        std::cerr << "An error occured when installing" << std::endl;
        return 1;
    }
    return 0;
}
