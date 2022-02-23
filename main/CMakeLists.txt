idf_component_register(
        SRCS
            "si_main.cpp"
            "uart_data_broker.cpp" "includes/uart_data_broker.hpp"
            "uart_config_broker.cpp" "includes/uart_config_broker.hpp"
            "uart_data_characteristic.cpp" "includes/uart_data_characteristic.hpp"

        INCLUDE_DIRS
            "includes"
)
