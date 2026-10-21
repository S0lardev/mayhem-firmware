#include "ui_tower_search.hpp"

#include "string_format.hpp"

namespace ui::external_app::tower_search {

TowerSearchRecentEntry::TowerSearchRecentEntry(
    RadioAccessTechnology rat,
    uint64_t data,
    uint16_t bits)
    : rat{rat}, bits{bits}, data{data} {
}

void TowerSearchRecentEntry::inc_age(uint16_t delta) {
    if (delta <= UINT16_MAX - age)
        age += delta;
    else
        age = UINT16_MAX;
}

void TowerSearchRecentEntry::reset_age() {
    age = 0;
}

std::string TowerSearchRecentEntry::to_csv() const {
    return ";" + std::to_string(static_cast<uint8_t>(rat)) +
           ";" + std::to_string(bits) +
           ";" + to_string_hex(data, 16) + ";";
}

Optional<File::Error> TowerLogger::append(
    const std::filesystem::path& filename) {
    return log_file.append(filename);
}

void TowerLogger::log_data(const TowerSearchRecentEntry& data) {
    log_file.write_entry(data.to_csv());
}

void TowerLogger::write_header() {
    log_file.write_entry(";RAT;Bits;Data;");
}

TowerSearchView::TowerSearchView(NavigationView& nav)
    : nav_{nav} {
}

void TowerSearchView::configure_scan(
    RadioAccessTechnology rat,
    bool enabled) {
    switch (rat) {
        case RadioAccessTechnology::umts:
            umts_enabled = enabled;
            break;
        case RadioAccessTechnology::lte:
            lte_enabled = enabled;
            break;
        case RadioAccessTechnology::nr:
            nr_enabled = enabled;
            break;
    }
}

void TowerSearchView::start_scan() {
    if (scanning)
        return;

    scanning = true;
    progress = 0;
    start_scan_thread();
}

void TowerSearchView::stop_scan() {
    if (!scanning)
        return;

    scanning = false;
    stop_scan_thread();
}

void TowerSearchView::scan_next_frequency() {
    if (!scanning)
        return;

    // Select the next band/channel and configure receiver_model here.
}

bool TowerSearchView::detect_sync_signal(
    const std::array<uint8_t, 256>& samples) const {
    // Implement RAT-specific synchronization detection here.
    (void)samples;
    return false;
}

bool TowerSearchView::decode_broadcast_channel(
    const std::array<uint8_t, 256>& samples,
    TowerSearchResult& result) const {
    // Implement BCH/MIB/SIB decoding for the selected RAT here.
    (void)samples;
    (void)result;
    return false;
}

void TowerSearchView::handle_scan_result(const TowerSearchResult& result) {
    TowerSearchRecentEntry entry{result.rat, result.cell_id, 0};
    if (logging_enabled)
        logger.log_data(entry);
}

void TowerSearchView::refresh() {
    scan_next_frequency();
}

void TowerSearchView::update() {
    if (scanning)
        refresh();
}

void TowerSearchView::start_scan_thread() {
    // Start or configure the future baseband capture/decoder worker here.
}

void TowerSearchView::stop_scan_thread() {
    // Stop the future baseband capture/decoder worker here.
}

void TowerSearchView::on_data(const TowerSearchRecentEntry* data) {
    if (data != nullptr && logging_enabled)
        logger.log_data(*data);
}

void TowerSearchView::focus() {
}

}  // namespace ui::external_app::tower_search
