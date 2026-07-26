#include "ui_tower_search.hpp"
#include "portapack.hpp"
#include "audio.hpp"
#include "baseband_api.hpp"
#include "string_format.hpp"
#include "file_path.hpp"
#include "ui_textentry.hpp"
#include "../keeloq_keystore.hpp"
#include "../keeloq_file.hpp"
#include "../keeloq_common.hpp"
#include "portapack_persistent_memory.hpp"
#include <cstring>

using namespace portapack;
using namespace ui;

namespace ui
{

std::string TowerSearchRecentEntry::to_csv() {
    std::string csv = ";";
    csv += SubGhzDView::getSensorTypeName((FPROTO_SUBGHZD_SENSOR)sensorType);
    csv += ";" + to_string_dec_uint(bits) + ";";
    csv += to_string_hex(data, 64 / 4);
    return csv;
}

    TowerSearchView::TowerSearchView(NavigationView &nav) // Application Main
    {
         // App code
    }

    void TowerSearchView::update()                   // Every time you get a DisplayFrameSync message this function will be ran
    {
         // Message code
    }
}