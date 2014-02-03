#include "logxx/logxx.h"
#include "dicom/dicom.h"
#include "dir.h"
#include <fstream>

static logxx::Log cLog("extractor");

std::ostream& operator<<(std::ostream& s, const Dicom & d){
        s << d.GetId() << "," << d.GetDate() << "," << d.GetName() << "," << d.GetPet();
        return s;
}

int main(int argc, char **argv){
	S_LOG("main");
        logxx::GlobalLogLevel(logxx::warning);
        if (argc < 2){
                log(logxx::error) << "A path with dicom files should be specified" << logxx::endl;
                return 1;
        } else {
                std::string path(argv[1]);                
                if (path[path.length()-1] != '/')
                        path += '/';
                
                std::string csvFile = path + "out.csv";
                std::ofstream out(csvFile);
                if (out.good()){
                        Dir dir(path, true, ".dcm");
                        if (dir.Ok()) {
                                std::string fName;
                                while ((fName = dir.Read()).empty() == false) {
                                        log(logxx::debug) << "{" << fName << "}" << logxx::endl;
                                        Dicom dicom(path + fName);
                                        if (dicom.Parse()){
                                                // cppcheck-suppress constStatement
                                                log(logxx::notice) << "{" << fName << "} Parsed" << logxx::endl;
                                                out << fName << "," << dicom << std::endl;

                                        } else {
                                                // cppcheck-suppress constStatement
                                                log(logxx::notice) << "{" << fName << "} Not parsed" << logxx::endl;
                                        }
                                }
                                return 0;
                        } else {
                                log(logxx::error) << "Can't open path {" << path << "}" << logxx::endl;
                                return 1;
                        }
                } else {
                        log(logxx::error) << "Can't open file {" << csvFile <<  "} for writing"  << logxx::endl;
                        return 1;
                }
        }
}
