#include "tools_tab.hpp"
#include "confirm_page.hpp"
#include "worker_page.hpp"
#include "cheats_page.hpp"
#include "payload_page.hpp"
#include "download_payload_page.hpp"
#include "changelog_page.hpp"
#include "JC_page.hpp"
#include "PC_page.hpp"
#include "net_page.hpp"
#include "extract.hpp"
#include "utils.hpp"
#include "fs.hpp"
#include "hide_tabs_page.hpp"
#include <json.hpp>
#include <filesystem>
#include <fstream>

namespace i18n = brls::i18n;
using namespace i18n::literals;
using json = nlohmann::json;

ToolsTab::ToolsTab(std::string tag, bool erista) : brls::List()
{
    if(!tag.empty() && tag != APP_VERSION){
        updateApp = new brls::ListItem("menus/tools/update_app"_i18n + tag +")");
        std::string text("menus/tools/dl_app"_i18n + std::string(APP_URL));
        updateApp->getClickEvent()->subscribe([&, text, tag](brls::View* view) {
            brls::StagedAppletFrame* stagedFrame = new brls::StagedAppletFrame();
            stagedFrame->setTitle("menus/common/updating"_i18n);
            stagedFrame->addStage(
                new ConfirmPage(stagedFrame, text)
            );
            stagedFrame->addStage(
                new WorkerPage(stagedFrame, "menus/common/downloading"_i18n, [](){util::downloadArchive(APP_URL, archiveType::app);})
            );
            stagedFrame->addStage(
                new WorkerPage(stagedFrame, "menus/common/extracting"_i18n, [tag](){util::extractArchive(archiveType::app, tag);})
            );
            stagedFrame->addStage(
                new ConfirmPage(stagedFrame, "menus/common/all_done"_i18n, true)
            );
            brls::Application::pushView(stagedFrame);
        });
        updateApp->setHeight(LISTITEM_HEIGHT);
        this->addView(updateApp);
    }

    cheats = new brls::ListItem("menus/tools/cheats"_i18n);
    cheats->getClickEvent()->subscribe([&](brls::View* view){
        brls::Application::pushView(new CheatsPage());
    });
    cheats->setHeight(LISTITEM_HEIGHT);
    this->addView(cheats);

    // ntcp = new brls::ListItem("menus/ntcp"_i18n);
    // ntcp->getClickEvent()->subscribe([&](brls::View* view){
    //     std::string res = syncTime();
    //     brls::Dialog* dialog = new brls::Dialog(res);
    //     brls::GenericEvent::Callback callback = [dialog](brls::View* view) {
    //         dialog->close();
    //     };
    //     dialog->addButton("menus/common/ok"_i18n, callback);
    //     dialog->setCancelable(true);
    //     dialog->open();

    // });
    // ntcp->setHeight(LISTITEM_HEIGHT);
    // this->addView(ntcp); 

    browser = new brls::ListItem("menus/tools/browser"_i18n);
    browser->getClickEvent()->subscribe([&](brls::View* view){
        char url[0xc00] = {0};
        SwkbdConfig kbd;
        Result rc = swkbdCreate(&kbd, 0);
        if (R_SUCCEEDED(rc)) {
            swkbdConfigMakePresetDefault(&kbd);
            swkbdConfigSetGuideText(&kbd, "www.cheatslips.com e-mail");
            swkbdConfigSetInitialText(&kbd, "https://google.com");
            swkbdShow(&kbd, url, sizeof(url));
            swkbdClose(&kbd);
        }
        else {
            strcpy(url, "https://google.com");
        }
        int at = appletGetAppletType();
        std::string error = "";
        if(at == AppletType_Application) { // Running as a title
            WebCommonConfig conf;
            WebCommonReply out;
            rc = webPageCreate(&conf, url);
            if (R_FAILED(rc))
                error += "\uE016 Error starting Browser\n\uE016 Lookup error code for more info " + rc;
            webConfigSetJsExtension(&conf, true);
            webConfigSetPageCache(&conf, true);
            webConfigSetBootLoadingIcon(&conf, true);
            webConfigSetWhitelist(&conf, ".*");
            rc = webConfigShow(&conf, &out);
            if (R_FAILED(rc))
                error += "\uE016 Error starting Browser\n\uE016 Lookup error code for more info " + rc;
        } else { // Running under applet
            error += "\uE016 Running in applet mode/through a forwarder.\n\uE016 Please launch hbmenu by holding [R] on a game";
        }
        if(!error.empty()){
            brls::Dialog* dialog = new brls::Dialog(error);
            brls::GenericEvent::Callback callback = [dialog](brls::View* view) {
                dialog->close();
            };
            dialog->addButton("menus/common/ok"_i18n, callback);
            dialog->setCancelable(true);
            dialog->open();
        }
        
    });
    browser->setHeight(LISTITEM_HEIGHT);
    this->addView(browser);

    // cleanUp = new brls::ListItem("menus/tools/clean_up"_i18n);
    // cleanUp->getClickEvent()->subscribe([&](brls::View* view){
    //     std::filesystem::remove(AMS_ZIP_PATH);
    //     std::filesystem::remove(APP_ZIP_PATH);
    //     std::filesystem::remove(CFW_ZIP_PATH);
    //     std::filesystem::remove(FW_ZIP_PATH);
    //     std::filesystem::remove(CHEATS_ZIP_PATH);
    //     std::filesystem::remove(SIGPATCHES_ZIP_PATH);
    //     fs::removeDir(AMS_DIRECTORY_PATH);
    //     fs::removeDir(SEPT_DIRECTORY_PATH);
    //     fs::removeDir(FW_DIRECTORY_PATH);
    //     brls::Dialog* dialog = new brls::Dialog("menus/common/all_done"_i18n);
    //     brls::GenericEvent::Callback callback = [dialog](brls::View* view) {
    //         dialog->close();
    //     };
    //     dialog->addButton("menus/common/ok"_i18n, callback);
    //     dialog->setCancelable(true);
    //     dialog->open();
    // });
    // cleanUp->setHeight(LISTITEM_HEIGHT);
    // this->addView(cleanUp);

    language = new brls::ListItem("menus/tools/language"_i18n);
    language->getClickEvent()->subscribe([&](brls::View* view){
        std::vector<std::pair<std::string, std::string>> languages{
            std::make_pair("menus/language/en-US"_i18n, "en-US"),
            std::make_pair("menus/language/ja"_i18n, "ja"),
            std::make_pair("menus/language/fr"_i18n, "fr"),
            std::make_pair("menus/language/de"_i18n, "de"),
            std::make_pair("menus/language/it"_i18n, "it"),
            std::make_pair("menus/language/es"_i18n, "es"),
            std::make_pair("menus/language/pl"_i18n, "pl"),
            std::make_pair("menus/language/zh-CN"_i18n, "zh-CN"),
            std::make_pair("menus/language/zh-TW"_i18n, "zh-TW"),
            std::make_pair("menus/language/es-419"_i18n, "es-419"),
            std::make_pair("menus/language/zh-Hant"_i18n, "zh-Hant"),
            std::make_pair("menus/language/zh-Hans"_i18n, "zh-Hans")
        };
        brls::AppletFrame* appView = new brls::AppletFrame(true, true);
        brls::List* list = new brls::List();
        brls::ListItem* listItem;
        listItem = new brls::ListItem(fmt::format("{} ({})", "menus/language/system_default"_i18n, i18n::getCurrentLocale()));
        listItem->registerAction("menus/tools/language"_i18n, brls::Key::A, [this] {
            std::filesystem::remove(LANGUAGE_JSON);
            brls::Application::quit();
            return true;
        });
        list->addView(listItem);
        for(auto& l : languages){
            listItem = new brls::ListItem(l.first);
            listItem->registerAction("menus/tools/language"_i18n, brls::Key::A, [this, l] { 
                json updatedLanguage = json::object();
                updatedLanguage["language"] = l.second;
                std::ofstream out(LANGUAGE_JSON);
                out << updatedLanguage.dump();
                out.close();
                brls::Application::quit();
                return true;
            });
            list->addView(listItem);
        }
        appView->setContentView(list);
        brls::PopupFrame::open("menus/tools/language"_i18n, appView, "", "");
    });
    language->setHeight(LISTITEM_HEIGHT);
    this->addView(language);

    // changelog = new brls::ListItem("menus/tools/changelog"_i18n);
    // changelog->getClickEvent()->subscribe([&](brls::View* view){
    //     brls::PopupFrame::open("menus/tools/changelog"_i18n, new ChangelogPage(), "", "");
    // });
    // changelog->setHeight(LISTITEM_HEIGHT);
    // this->addView(changelog);

/*     language = new brls::ListItem("menus/Language_Option"_i18n);
    language->getClickEvent()->subscribe([&](brls::View* view){
        brls::Application::pushView(new LanguageOptionPage());
    });
    language->setHeight(LISTITEM_HEIGHT);
    this->addView(language); */
}

