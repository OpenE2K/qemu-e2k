#ifndef E2K_TARGET_STRUCTS_H
#define E2K_TARGET_STRUCTS_H

struct target_ipc_perm {
    abi_int  __key;
    abi_uint  uid;
    abi_uint  gid;
    abi_uint  cuid;
    abi_uint  cgid;
    abi_uint mode;
    abi_ushort  __seq;
};

struct target_shmid_ds {
    struct target_ipc_perm     shm_perm;
    abi_int         shm_segsz;
    abi_long            shm_atime;
    abi_long            shm_dtime;
    abi_long            shm_ctime;
    abi_int  shm_cpid;
    abi_int  shm_lpid;
    abi_ushort          shm_nattch;
    abi_ushort          shm_unused;
    void            *shm_unused2;
    void            *shm_unused3;
};

#endif
