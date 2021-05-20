#pragma once

constexpr const char ROOT_PATH[]=           "/";
constexpr const char APP_PATH[] =           "/switch/kefirupdater/";
constexpr const char NRO_PATH[] =           "/switch/kefirupdater/kefirupdater.nro";
constexpr const char DOWNLOAD_PATH[] =      "/switch/kefirupdater/";
constexpr const char CONFIG_PATH[] =        "/switch/kefirupdater/";
constexpr const char CONFIG_PATH_UNZIP[] =   "config\\kefirupdater";

constexpr const char RCM_PAYLOAD_PATH[] =   "romfs:/aio_rcm.bin";

constexpr const char APP_URL[] =            "https://github.com/rashevskyv/kefirupdater/releases/latest/download/kefirupdater.zip";
constexpr const char TAGS_INFO[] =           "https://api.github.com/repos/HamletDuFromage/kefirupdater/releases/latest";
constexpr const char APP_FILENAME[] =        "/switch/kefirupdater/app.zip";

constexpr const char SIGPATCHES_URL[] =     "https://raw.githubusercontent.com/rashevskyv/nx-links/master/sigpatches.json";
constexpr const char SIGPATCHES_FILENAME[] ="/switch/kefirupdater/sigpatches.zip";
constexpr const char HEKATE_IPL_PATH[] =    "/bootloader/hekate_ipl.ini";

constexpr const char FIRMWARE_URL[] =       "https://raw.githubusercontent.com/rashevskyv/nx-links/master/firmwares.json";
constexpr const char FIRMWARE_FILENAME[] =  "/switch/kefirupdater/firmware.zip";
constexpr const char FIRMWARE_PATH[] =      "/firmware/";

constexpr const char CFW_URL[] =            "https://raw.githubusercontent.com/rashevskyv/nx-links/master/cfw.json";
constexpr const char CFW_FILENAME[] =       "/switch/kefirupdater/kefir.zip";

constexpr const char AMS_URL[] =            "https://raw.githubusercontent.com/rashevskyv/nx-links/master/ams.json";
constexpr const char SXOS_URL[] =           "https://raw.githubusercontent.com/rashevskyv/nx-links/master/sxos.json";
constexpr const char AMS_FILENAME[] =       "/switch/kefirupdater/ams.zip";

constexpr const char HEKATE_URL[] =         "https://raw.githubusercontent.com/rashevskyv/nx-links/master/hekate.json";

constexpr const char PAYLOAD_URL[] =        "https://raw.githubusercontent.com/rashevskyv/nx-links/master/payloads.json";

constexpr const char CHEATS_RELEASE_URL[] = "https://github.com/HamletDuFromage/switch-cheats-db/releases/tag/v1.0";
constexpr const char CHEATS_URL_TITLES[] =  "https://github.com/HamletDuFromage/switch-cheats-db/releases/download/v1.0/titles.zip";
constexpr const char CHEATS_URL_CONTENTS[] ="https://github.com/HamletDuFromage/switch-cheats-db/releases/download/v1.0/contents.zip";
constexpr const char CHEATS_URL_VERSION[] = "https://github.com/HamletDuFromage/switch-cheats-db/releases/download/v1.0/VERSION";
constexpr const char LOOKUP_TABLE_URL[] =   "https://raw.githubusercontent.com/HamletDuFromage/switch-cheats-db/master/versions.json";
constexpr const char LOOKUP_TABLE_CBOR[] =  "https://github.com/HamletDuFromage/switch-cheats-db/raw/master/versions.cbor";
constexpr const char VERSIONS_DIRECTORY[] = "https://raw.githubusercontent.com/HamletDuFromage/switch-cheats-db/master/versions/";
constexpr const char CHEATSLIPS_CHEATS_URL[] ="https://www.cheatslips.com/api/v1/cheats/";
constexpr const char CHEATSLIPS_TOKEN_URL[] ="https://www.cheatslips.com/api/v1/token";
constexpr const char TOKEN_PATH[] =         "/switch/kefirupdater/token.json";
constexpr const char CHEATS_FILENAME[] =    "/switch/kefirupdater/cheats.zip";
constexpr const char CHEATS_EXCLUDE[] =     "/switch/kefirupdater/exclude.txt";
constexpr const char FILES_IGNORE[] =       "/switch/kefirupdater/preserve.txt";
constexpr const char INTERNET_JSON[] =      "/switch/kefirupdater/internet.json";
constexpr const char UPDATED_TITLES_PATH[] ="/switch/kefirupdater/updated.dat";
constexpr const char CHEATS_VERSION[] =     "/switch/kefirupdater/cheats_version.dat";
constexpr const char AMS_CONTENTS[] =       "/atmosphere/contents/";
constexpr const char REINX_CONTENTS[] =     "/ReiNX/contents/";
constexpr const char SXOS_TITLES[] =        "/sxos/titles/";
constexpr const char AMS_PATH[] =           "/atmosphere/";
constexpr const char SXOS_PATH[] =          "/sxos/";
constexpr const char REINX_PATH[] =         "/ReiNX/";
constexpr const char CONTENTS_PATH[] =      "contents/";
constexpr const char TITLES_PATH[] =        "titles/";

constexpr const char COLOR_PROFILES_PATH[] ="/switch/kefirupdater/jc_profiles.json";
constexpr const char PC_COLOR_PATH[] =      "/switch/kefirupdater/pc_profiles.json";

constexpr const char PAYLOAD_PATH[] =       "/payloads/";
constexpr const char BOOTLOADER_PATH[] =    "/bootloader/";
constexpr const char BOOTLOADER_PL_PATH[] = "/bootloader/payloads/";
constexpr const char UPDATE_BIN_PATH[] =    "/bootloader/update.bin";
constexpr const char REBOOT_PAYLOAD_PATH[] ="/payload.bin";
constexpr const char FUSEE_SECONDARY[] =    "/atmosphere/fusee-secondary.bin";
constexpr const char FUSEE_MTC[] =          "/atmosphere/fusee-mtc.bin";

constexpr const char AMS_ZIP_PATH[] =           "/switch/kefirupdater/ams.zip";
constexpr const char KEFIR_ZIP_PATH[] =         "/switch/kefirupdater/kefir.zip";
constexpr const char APP_ZIP_PATH[] =           "/switch/kefirupdater/app.zip";
constexpr const char CFW_ZIP_PATH[] =           "/switch/kefirupdater/cfw.zip";
constexpr const char FW_ZIP_PATH[] =            "/switch/kefirupdater/firmware.zip";
constexpr const char CHEATS_ZIP_PATH[] =        "/switch/kefirupdater/cheats.zip";
constexpr const char SIGPATCHES_ZIP_PATH[] =    "/switch/kefirupdater/sigpatches.zip";
constexpr const char AMS_DIRECTORY_PATH[] =     "/switch/kefirupdater/atmosphere/";
constexpr const char KEFIR_DIRECTORY_PATH[] =   "/kefir/";
constexpr const char SEPT_DIRECTORY_PATH[] =    "/switch/kefirupdater/sept/";
constexpr const char FW_DIRECTORY_PATH[] =      "/firmware/";

constexpr const char HIDE_TABS_JSON[] =     "/switch/kefirupdater/hide_tabs.json";
constexpr const char COPY_FILES_JSON[] =    "/switch/kefirupdater/copy_files.json";
constexpr const char LANGUAGE_JSON[] =      "/switch/kefirupdater/language.json";
constexpr const char HOMEBREW[] =           "/switch/kefirupdater/language.json";

constexpr const char ROMFS_FORWARDER[] =    "romfs:/aiosu-forwarder.nro";
constexpr const char FORWARDER_PATH[] =     "/switch/kefirupdater/aiosu-forwarder.nro";

constexpr const char HIDDEN_AIO_FILE[] =    "/switch/kefirupdater/.kefirupdater";

constexpr const int LISTITEM_HEIGHT =       50;


enum class archiveType{
    sigpatches,
    cheats,
    fw,
    app,
    cfw,
    ams_cfw,
};

enum class CFW{
    rnx,
    sxos,
    ams,
};