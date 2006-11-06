MACHINE_START(GP2X, "GameparkHoldings GP2X")
        .phys_io        = NIO_START,
        .io_pg_offst    = ((NIO_BASE) >> 18) & 0xfffc,
        .boot_params    = 0x00000100,
MACHINE_END
