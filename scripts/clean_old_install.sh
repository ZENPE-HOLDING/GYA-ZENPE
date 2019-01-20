#! /bin/bash

if [ -d "/usr/local/include/gyaio" ]; then
   printf "\n\tOld gyaio install needs to be removed.\n\n"
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            if [ "$(id -u)" -ne 0 ]; then
               printf "\n\tThis requires sudo, please run ./scripts/clean_old_install.sh with sudo\n\n"
               exit -1
            fi
            pushd /usr/local &> /dev/null

            pushd include &> /dev/null
            rm -rf appbase chainbase gyaio gyaio.system gyaiolib fc libc++ musl &> /dev/null
            popd &> /dev/null

            pushd bin &> /dev/null
            rm clgya gyaio-abigen gyaio-applesdemo gyaio-launcher gyaio-s2wasm gyaio-wast2wasm gyaiocpp kgyad nodgya &> /dev/null
            popd &> /dev/null

            libraries=(libgyaio_testing
            libgyaio_chain
            libfc
            libbinaryen
            libWAST
            libWASM
            libRuntime
            libPlatform
            libIR
            libLogging
            libsoftfloat
            libchainbase
            libappbase
            libbuiltins)
            pushd lib &> /dev/null
            for lib in ${libraries[@]}; do
               rm ${lib}.a ${lib}.dylib ${lib}.so &> /dev/null
            done
            popd &> /dev/null

            pushd etc &> /dev/null
            rm gyaio &> /dev/null
            popd &> /dev/null

            pushd share &> /dev/null
            rm gyaio &> /dev/null
            popd &> /dev/null

            pushd usr/share &> /dev/null
            rm gyaio &> /dev/null
            popd &> /dev/null

            pushd var/lib &> /dev/null
            rm gyaio &> /dev/null
            popd &> /dev/null

            pushd var/log &> /dev/null
            rm gyaio &> /dev/null
            popd &> /dev/null

            popd &> /dev/null
            break;;
         [Nn]* )
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi
