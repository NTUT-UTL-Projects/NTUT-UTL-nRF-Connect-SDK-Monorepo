# NTUT-UTL-nRF-Connect-SDK-Monorepo

Refers to [GitHub - XIAN-SHENG-576692/nRF-Connect-SDK-Environment · GitHub](https://github.com/XIAN-SHENG-576692/nRF-Connect-SDK-Environment)

1. Run this command on the host
    ```bash
    ./dev_setup_scripts/create_config_env.sh -t v3.3.0-rc2
    ```

2. Run this command in the container
    ```bash
    ./dev_setup_scripts/fix_jlink_error.sh
    ./dev_setup_scripts/install_ncs_in_volume.sh -m https://github.com/nrfconnect/sdk-nrf.git --mr v3.2.4
    ./dev_setup_scripts/install_tools_for_native_building.sh
    ```
