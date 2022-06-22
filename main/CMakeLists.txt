idf_component_register(
        SRCS
            "si_main.cpp"
            "driver/uart_ctrl.cpp" "driver/uart_ctrl.hpp"
            "storage/config_manager.cpp" "storage/config_manager.hpp"
            "misc/eternal.hpp" "misc/config_constants.hpp"
            "protocol/ble_manager.cpp" "protocol/ble_manager.hpp"
            "protocol/chunk_codec.cpp" "protocol/chunk_codec.hpp"
            "protocol/config_handler.cpp" "protocol/config_handler.hpp"
            "protocol/uart_handler.cpp" "protocol/uart_handler.hpp"

        INCLUDE_DIRS
            "driver" "storage" "protocol" "misc"
)
