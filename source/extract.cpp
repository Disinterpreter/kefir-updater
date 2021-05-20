#include "extract.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <set>
#include <unzipper.h>
#include "progress_event.hpp"
#include "utils.hpp"
#include "download.hpp"
#include "fs.hpp"

namespace i18n = brls::i18n;
using namespace i18n::literals;

namespace extract {

    namespace {
        bool caselessCompare (const std::string& a, const std::string& b){
            return strcasecmp(a.c_str(), b.c_str()) < 0;
        }
    }

void extract(const char * filename, const char* workingPath, int overwriteInis){
    ProgressEvent::instance().reset();
    ProgressEvent::instance().setStep(1);
    chdir(workingPath);
    std::set<std::string> ignoreList = fs::readLineByLine(FILES_IGNORE);
    std::set<std::string>::iterator it;
    zipper::Unzipper unzipper(filename);
    std::vector<zipper::ZipEntry> entries = unzipper.entries();
    int k = -1;
    bool isIgnored;
    ProgressEvent::instance().setTotalSteps(entries.size() + 1);
    for (int i = 0; i < (int) entries.size(); i++){
        isIgnored = false;
        it = ignoreList.begin();
        if(overwriteInis == 0 && entries[i].name.substr(entries[i].name.length() - 4) == ".ini"){
            if(!std::filesystem::exists("/" + entries[i].name)){
                unzipper.extractEntry(entries[i].name);
            }
            continue;
        }
        while (it != ignoreList.end() ){
            k = ("/" + entries[i].name).find((*it));
            if(k == 0 || k == 1){
                isIgnored = false;
                if(!std::filesystem::exists("/" + entries[i].name)){
                    unzipper.extractEntry(entries[i].name);
                }
                break;
            }
            it++;
        }
                unzipper.extractEntry(entries[i].name);
                if(entries[i].name.substr(0, 13) == "hekate_ctcaer") {
                    fs::copyFile(("/" + entries[i].name).c_str(), UPDATE_BIN_PATH);
                }

        
        ProgressEvent::instance().setStep(i);
    }
    unzipper.close();
    ProgressEvent::instance().setStep(ProgressEvent::instance().getMax());
}

void extract(const char * filename, const char* workingPath, const char* toExclude){
    ProgressEvent::instance().reset();
    ProgressEvent::instance().setStep(1);
    chdir(workingPath);
    std::set<std::string> ignoreList = fs::readLineByLine(FILES_IGNORE);
    ignoreList.insert(toExclude);
    std::set<std::string>::iterator it;
    zipper::Unzipper unzipper(filename);
    std::vector<zipper::ZipEntry> entries = unzipper.entries();
    int k = -1;
    bool isIgnored;
    ProgressEvent::instance().setTotalSteps(entries.size() + 1);
    for (int i = 0; i < (int) entries.size(); i++){
        isIgnored = false;
        it = ignoreList.begin();
        while (it != ignoreList.end()){
            k = ("/" + entries[i].name).find((*it));
            if(k == 0 || k == 1){
                isIgnored = true;
                if(!std::filesystem::exists("/" + entries[i].name)){
                    unzipper.extractEntry(entries[i].name);
                }
                break;
            }
            it++;
        }
        if(!isIgnored) {
            unzipper.extractEntry(entries[i].name);
        }
        
        ProgressEvent::instance().setStep(i);
    }
    unzipper.close();
    ProgressEvent::instance().setStep(ProgressEvent::instance().getMax());
}

std::vector<std::string> getInstalledTitlesNs(){
    std::vector<std::string> titles;

    NsApplicationRecord *records = new NsApplicationRecord[MaxTitleCount]();
    NsApplicationControlData *controlData = NULL;
    NacpLanguageEntry* langEntry = NULL;

    s32 recordCount     = 0;
    u64 controlSize     = 0;

    if (R_SUCCEEDED(nsListApplicationRecord(records, MaxTitleCount, 0, &recordCount))){
        for (s32 i = 0; i < recordCount; i++){
            controlSize = 0;
            controlData = (NsApplicationControlData*)malloc(sizeof(NsApplicationControlData));
            if(controlData != NULL)
                memset(controlData, 0, sizeof(NsApplicationControlData));

            if(R_FAILED(nsGetApplicationControlData(NsApplicationControlSource_Storage, records[i].application_id, controlData, sizeof(NsApplicationControlData), &controlSize))) continue;

            if(controlSize < sizeof(controlData->nacp)) continue;

            if(R_FAILED(nacpGetLanguageEntry(&controlData->nacp, &langEntry))) continue;

            titles.push_back(util::formatApplicationId(records[i].application_id));
        }
    }
    free(controlData);
    delete[] records;
    std::sort(titles.begin(), titles.end());
    return titles;
}

std::vector<std::string> excludeTitles(const char* path, std::vector<std::string> listedTitles){
    std::vector<std::string> titles;
    std::ifstream file(path);
    int total = 0;
    std::string name;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::transform(line.begin(), line.end(), line.begin(), ::toupper);
            for(size_t i = 0; i < listedTitles.size(); i++) {
                if(line == listedTitles[i]) {
                    titles.push_back(line);
                    break;
                }
            }
        }
        file.close();
    }

    std::sort(titles.begin(), titles.end());

    std::vector<std::string> diff;
    std::set_difference(listedTitles.begin(), listedTitles.end(), titles.begin(), titles.end(), 
                         std::inserter(diff, diff.begin()));
    return diff;
}

void extractCheats(const char * zipPath, std::vector<std::string> titles, CFW cfw, bool credits){
    //TODO: REWRITE WITH SETS INSTEAD OF VECTORS
    ProgressEvent::instance().reset();
    zipper::Unzipper unzipper(zipPath);
    std::vector<zipper::ZipEntry> entries = unzipper.entries();
    std::set<std::string> extractedTitles;
    int offset = 0;
    switch(cfw){
        case CFW::ams:
            offset = std::string(CONTENTS_PATH).length();
            std::filesystem::create_directory(AMS_PATH);
            std::filesystem::create_directory(AMS_CONTENTS);
            chdir(AMS_PATH);
            break;
        case CFW::rnx:
            offset = std::string(CONTENTS_PATH).length();
            std::filesystem::create_directory(REINX_PATH);
            std::filesystem::create_directory(REINX_CONTENTS);
            chdir(REINX_PATH);
            break;
        case CFW::sxos:
            offset = std::string(TITLES_PATH).length();
            std::filesystem::create_directory(SXOS_PATH);
            std::filesystem::create_directory(SXOS_TITLES);
            chdir(SXOS_PATH);
            break;
    }

    std::vector<std::string> entriesNames;
    std::vector<int> parentIndexes;
    for (size_t i = 1; i < entries.size(); i++){
        entriesNames.push_back(entries[i].name);
    }

    std::sort(entriesNames.begin(), entriesNames.end(), caselessCompare);

    std::vector<std::string> parents;
    std::vector<std::vector<std::string>> children;
    std::vector<std::string> tempChildren;


    size_t k = 0;
    while(k < entriesNames.size()){
        if(entriesNames[k].length() == (size_t) (offset + 17)){
            parents.push_back(entriesNames[k]);
            k++;
            while(entriesNames[k].length() != (size_t) (offset + 17) && k < entriesNames.size()){
                if(credits == false){ 
                    if(strcasecmp(entriesNames[k].substr(offset + 16, 7).c_str(), "/cheats") == 0){
                        tempChildren.push_back(entriesNames[k]);
                    }
                }
                else{
                    tempChildren.push_back(entriesNames[k]);
                }
                k++;
            }
            children.push_back(tempChildren);
            tempChildren.clear (); 
        }
        else{
            k++;
        }
    }

    size_t lastL = 0;
    std::string id;
    ProgressEvent::instance().setTotalSteps(titles.size());
    for(size_t j = 0; j < titles.size(); j++){
        for(size_t l = lastL; l < parents.size(); l++){
            if(strcasecmp((titles[j]).c_str(), parents[l].substr(offset, 16).c_str()) == 0){
                unzipper.extractEntry(parents[l]);
                for(auto& e : children[l]){
                    unzipper.extractEntry(e);
                    extractedTitles.insert(id);
                    ProgressEvent::instance().setStep(j);
                    id = e.substr(offset, 16);
                    std::transform(id.begin(), id.end(), id.begin(), ::toupper);
                }
                lastL = l;
                break;
            }
        }
    }
    unzipper.close();
    writeTitlesToFile(extractedTitles, UPDATED_TITLES_PATH);
    auto cheatsVerVec = download::downloadFile(CHEATS_URL_VERSION);
    std::string cheatsVer(cheatsVerVec.begin(), cheatsVerVec.end());
    util::saveVersion(cheatsVer, CHEATS_VERSION);
    ProgressEvent::instance().setStep(ProgressEvent::instance().getMax());
}

void extractAllCheats(const char * zipPath, CFW cfw){
    ProgressEvent::instance().reset();
    zipper::Unzipper unzipper(zipPath);
    std::vector<zipper::ZipEntry> entries = unzipper.entries();
    int offset = 0;
    switch(cfw){
        case CFW::ams:
            offset = std::string(CONTENTS_PATH).length() + 17 + 7;
            std::filesystem::create_directory(AMS_PATH);
            std::filesystem::create_directory(AMS_CONTENTS);
            chdir(AMS_PATH);
            break;
        case CFW::rnx:
            offset = std::string(CONTENTS_PATH).length() + 17 + 7;
            std::filesystem::create_directory(REINX_PATH);
            std::filesystem::create_directory(REINX_CONTENTS);
            chdir(REINX_PATH);
            break;
        case CFW::sxos:
            offset = std::string(TITLES_PATH).length() + 17 + 7;
            std::filesystem::create_directory(SXOS_PATH);
            std::filesystem::create_directory(SXOS_TITLES);
            chdir(SXOS_PATH);
            break;
    }
    ProgressEvent::instance().setTotalSteps(entries.size());
    for(size_t j = 0; j < entries.size(); j++){
        if(((int) entries[j].name.size() == offset + 16 + 4) && (isBID(entries[j].name.substr(offset, 16)))) {
            unzipper.extractEntry(entries[j].name);
        }
        ProgressEvent::instance().setStep(j);
    }
    unzipper.close();
    auto cheatsVerVec = download::downloadFile(CHEATS_URL_VERSION);
    std::string cheatsVer(cheatsVerVec.begin(), cheatsVerVec.end());
    util::saveVersion(cheatsVer, CHEATS_VERSION);
    ProgressEvent::instance().setStep(ProgressEvent::instance().getMax());
}

bool isBID(std::string bid) {
    for(char const &c : bid){
        if(!isxdigit(c)) return false;
    }
    return true;
}

void writeTitlesToFile(std::set<std::string> titles, const char* path){
    std::ofstream updatedTitlesFile;
    std::set<std::string>::iterator it = titles.begin();
    updatedTitlesFile.open(path, std::ofstream::out | std::ofstream::trunc);
    if(updatedTitlesFile.is_open()) {
        while (it != titles.end()){
            updatedTitlesFile << (*it) << std::endl;
            it++;
        }
        updatedTitlesFile.close();
    }
}

void removeCheats(CFW cfw){
    std::string path;
    switch(cfw){
        case CFW::ams:
            path = std::string(AMS_PATH) + std::string(CONTENTS_PATH);
            break;
        case CFW::rnx:
            path = std::string(REINX_PATH) + std::string(CONTENTS_PATH);
            break;
        case CFW::sxos:
            path = std::string(SXOS_PATH) + std::string(TITLES_PATH);
            break;
    }
    ProgressEvent::instance().reset();
    ProgressEvent::instance().setTotalSteps(std::distance(std::filesystem::directory_iterator(path), std::filesystem::directory_iterator()));
    int c = 0;
    for (const auto & entry : std::filesystem::directory_iterator(path)){
        std::string cheatsPath =  entry.path().string() + "/cheats";
        if(std::filesystem::exists(cheatsPath)){
            for (const auto & cheat : std::filesystem::directory_iterator(cheatsPath)){
                std::filesystem::remove(cheat);
            }
            rmdir(cheatsPath.c_str());
            if(std::filesystem::is_empty(entry)){
                rmdir(entry.path().string().c_str());
            }
        }
        ProgressEvent::instance().setStep(c++);
    }
    std::filesystem::remove(UPDATED_TITLES_PATH);
    std::filesystem::remove(CHEATS_VERSION);
    ProgressEvent::instance().setStep(ProgressEvent::instance().getMax());
}

}