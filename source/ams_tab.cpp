#include "ams_tab.hpp"
#include "download.hpp"
#include "extract.hpp"
#include "confirm_page.hpp"
#include "dialogue_page.hpp"
#include "worker_page.hpp"
#include "utils.hpp"
#include "current_cfw.hpp"
#include <string>
 
namespace i18n = brls::i18n;
using namespace i18n::literals;

AmsTab::AmsTab() :
    brls::List()
{
    std::vector<std::pair<std::string, std::string>> links;
    std::string operation("menus/main/getting"_i18n);
    this->description = new brls::Label(brls::LabelStyle::MEDIUM, (CurrentCfw::running_cfw == CFW::ams ? "\n" + "menus/ams_update/current_ams"_i18n + CurrentCfw::getAmsInfo() : ""), true);
    this->addView(description);
    operation += "menus/main/ams"_i18n;
    links = download::getLinks(CFW_URL);

    this->size = links.size();
    if(this->size){
        auto hekate_link = download::getLinks(HEKATE_URL);
        std::string hekate_url = hekate_link[0].second;
        std::string text_hekate = "menus/common/download"_i18n + hekate_link[0].first;

        for (const auto& link : links){
            std::string url = link.second;
            std::string text("menus/common/download"_i18n + link.first + "menus/common/from"_i18n + url);
            listItem = new brls::ListItem(link.first);
            listItem->setHeight(LISTITEM_HEIGHT);
            listItem->getClickEvent()->subscribe([&, text, text_hekate, url, hekate_url, operation](brls::View* view) {
                brls::StagedAppletFrame* stagedFrame = new brls::StagedAppletFrame();
                stagedFrame->setTitle(operation);
                stagedFrame->addStage(
                    new ConfirmPage(stagedFrame, text)
                );
                stagedFrame->addStage(
                    new WorkerPage(stagedFrame, "menus/common/downloading"_i18n, [url](){util::downloadArchive(url, archiveType::cfw);})
                );
                stagedFrame->addStage(
                    new WorkerPage(stagedFrame, "menus/common/extracting"_i18n, [](){util::extractArchive(archiveType::cfw);})
                );
                stagedFrame->addStage(
                    new ConfirmPage(stagedFrame, "menus/ams_update/reboot_rcm"_i18n, false, true)
                );
                brls::Application::pushView(stagedFrame);
            });
            this->addView(listItem);
        }
    }
    else{
        notFound = new brls::Label(
            brls::LabelStyle::DESCRIPTION,
            "menus/main/links_not_found"_i18n,
            true
        );
        notFound->setHorizontalAlign(NVG_ALIGN_CENTER);
        this->addView(notFound);
    }
}

brls::View* AmsTab::getDefaultFocus()
{
    if(this->size)
        return this->brls::List::getDefaultFocus();
    else
        return nullptr;
}