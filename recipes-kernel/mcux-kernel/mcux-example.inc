# Copyright 2022 NXP

require mcux-sdk-src.inc
require mcux-sdk-common.inc

inherit deploy

do_configure[noexec] = "1"
# do_rootfs[noexec] = "1"
# do_package[noexec] = "1"
do_package_qa[noexec] = "1"
#do_packagedata[noexec] = "1"
# do_package_write_ipk[noexec] = "1"
# do_package_write_deb[noexec] = "1"
# do_package_write_rpm[noexec] = "1"

INHIBIT_DEFAULT_DEPS = "1"
# INHIBIT_SYSROOT_STRIP = "1"
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

DEPENDS = "gcc-arm-none-eabi-native"
ARMGCC_DIR ?= "${WORKDIR}/recipe-sysroot-native${datadir}/arm-none-eabi"

EXAMPLE_ARMGCC_DIR = "${S}/${MCUX_EXAMPLE_DIR}/armgcc"

do_compile() {
    export ARMGCC_DIR="${ARMGCC_DIR}"
    echo "Start to compile example with ${ARMGCC_DIR}"
    cd ${EXAMPLE_ARMGCC_DIR}

    chmod u+x *.sh
    ./clean.sh

    # build all
    # ./build_all.sh

    # build targets
    for target in ${EXAMPLE_TARGET_TYPE}; do
        if [ -f "build_${target}.sh" ]; then
            ./"build_${target}.sh"
        fi
    done
}

FILES_${PN} += "/examples/*"

do_install () {
    install -d ${D}/examples/${PN}

    for target in ${EXAMPLE_TARGET_TYPE}; do
        if [ -d "${EXAMPLE_ARMGCC_DIR}/${target}" ]; then
            cp -r ${EXAMPLE_ARMGCC_DIR}/${target}  ${D}/examples/${PN}
        fi
    done
}

do_deploy () {
    install -d ${DEPLOYDIR}/examples/${PN}

    for target in ${EXAMPLE_TARGET_TYPE}; do
        if [ -d "${EXAMPLE_ARMGCC_DIR}/${target}" ]; then
            cp -r ${EXAMPLE_ARMGCC_DIR}/${target}  ${DEPLOYDIR}/examples/${PN}
        fi
    done
}

addtask deploy after do_compile