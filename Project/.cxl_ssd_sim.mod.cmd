savedcmd_cxl_ssd_sim.mod := printf '%s\n'   cxl_ssd_sim.o | awk '!x[$$0]++ { print("./"$$0) }' > cxl_ssd_sim.mod
