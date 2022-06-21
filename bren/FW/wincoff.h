/**
 ** Structures that appear in a file
 **/

struct msdos_header
{
    br_uint_16 magic;
    br_uint_16 last_page_bytes;
    br_uint_16 pages;
    br_uint_16 n_relocations;
    br_uint_16 header_size;
    br_uint_16 minalloc;
    br_uint_16 maxalloc;
    br_uint_16 start_ss;
    br_uint_16 start_sp;
    br_uint_16 checksum;
    br_uint_16 start_ip;
    br_uint_16 start_cs;
    br_uint_16 reloc_offset;
    br_uint_16 overlay_number;
    br_uint_16 _reserved0[4];
    br_uint_16 oem_id;
    br_uint_16 oem_info;
    br_uint_16 _reserved1[10];
    br_uint_32 new_header_offset;
};

struct coff_header
{
    br_uint_16 machine;
    br_uint_16 n_sections;
    br_uint_32 time_date;
    br_uint_32 symbols_offset;
    br_uint_32 n_symbols;
    br_uint_16 opt_header_size;
    br_uint_16 flags;
};

#define IMAGE_FILE_MACHINE_UNKNOWN 0
#define IMAGE_FILE_MACHINE_I386 0x14c
#define IMAGE_FILE_MACHINE_R3000 0x162
#define IMAGE_FILE_MACHINE_R4000 0x166
#define IMAGE_FILE_MACHINE_ALPHA 0x184
#define IMAGE_FILE_MACHINE_POWERPC 0x1F0

#define IMAGE_FILE_RELOCS_STRIPPED 0x0001
#define IMAGE_FILE_EXECUTABLE_IMAGE 0x0002
#define IMAGE_FILE_LINE_NUMS_STRIPPED 0x0004
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED 0x0008
#define IMAGE_FILE_BYTES_REVERSED_LO 0x0080
#define IMAGE_FILE_32BIT_MACHINE 0x0100
#define IMAGE_FILE_DEBUG_STRIPPED 0x0200
#define IMAGE_FILE_SYSTEM 0x1000
#define IMAGE_FILE_DLL 0x2000
#define IMAGE_FILE_BYTES_REVERSED_HI 0x8000

#define MAX_DATA_DIRECTORIES 16

struct nt_optional_header
{
    /*
     * Standard Win32 header
     */
    br_uint_16 magic;
    br_uint_8 l_major;
    br_uint_8 l_minor;
    br_uint_32 code_size;
    br_uint_32 data_size;
    br_uint_32 bss_size;
    br_uint_32 entry_point;
    br_uint_32 code_base;
    br_uint_32 data_base;

    /*
     * NT specific header
     */
    br_uint_32 image_base;
    br_uint_32 section_alignment;
    br_uint_32 file_alignment;
    br_uint_16 os_major;
    br_uint_16 os_minor;
    br_uint_16 user_major;
    br_uint_16 user_minor;
    br_uint_16 subsys_major;
    br_uint_16 subsys_minor;
    br_uint_32 _reserved;
    br_uint_32 image_size;
    br_uint_32 header_size;
    br_uint_32 file_checksum;
    br_uint_16 subsystem;
    br_uint_16 dll_flags;
    br_uint_32 stack_reserve_size;
    br_uint_32 stack_commit_size;
    br_uint_32 heap_reserve_size;
    br_uint_32 heap_commit_size;
    br_uint_32 loader_flags;
    br_uint_32 n_data_directories;
    struct
    {
        br_uint_32 rva;
        br_uint_32 size;
    } directories[MAX_DATA_DIRECTORIES];
};

#define IMAGE_SUBSYSTEM_UNKNOWN 0
#define IMAGE_SUBSYSTEM_NATIVE 1
#define IMAGE_SUBSYSTEM_WINDOWS_GUI 2
#define IMAGE_SUBSYSTEM_WINDOWS_CUI 3
#define IMAGE_SUBSYSTEM_OS2_CUI 5
#define IMAGE_SUBSYSTEM_POSIX_CUI 7

#define DIRECTORY_EXPORT 0
#define DIRECTORY_IMPORT 1
#define DIRECTORY_RESOURCE 2
#define DIRECTORY_EXCEPTION 3
#define DIRECTORY_SECURITY 4
#define DIRECTORY_BASERELOC 5
#define DIRECTORY_DEBUG 6
#define DIRECTORY_COPYRIGHT 7
#define DIRECTORY_GLOBALPTR 8
#define DIRECTORY_TLS 9
#define DIRECTORY_LOAD_CONFIG 10

struct section_header
{
    br_uint_8 section_name[8];
    br_uint_32 virtual_size;
    br_uint_32 rva;
    br_uint_32 data_size;
    br_uint_32 data_offset;
    br_uint_32 relocs_offset;
    br_uint_32 linenums_offset;
    br_uint_16 n_relocs;
    br_uint_16 n_linenums;
    br_uint_32 flags;
};

#define IMAGE_SCN_TYPE_NO_PAD 0x00000008

#define IMAGE_SCN_CNT_CODE 0x00000020
#define IMAGE_SCN_CNT_INITIALIZED_DATA 0x00000040
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA 0x00000080

#define IMAGE_SCN_LNK_OTHER 0x00000100
#define IMAGE_SCN_LNK_INFO 0x00000200
#define IMAGE_SCN_LNK_REMOVE 0x00000800
#define IMAGE_SCN_LNK_COMDAT 0x00001000

#define IMAGE_SCN_ALIGN_1BYTES 0x00100000
#define IMAGE_SCN_ALIGN_2BYTES 0x00200000
#define IMAGE_SCN_ALIGN_4BYTES 0x00300000
#define IMAGE_SCN_ALIGN_8BYTES 0x00400000
#define IMAGE_SCN_ALIGN_16BYTES 0x00500000
#define IMAGE_SCN_ALIGN_32BYTES 0x00600000
#define IMAGE_SCN_ALIGN_64BYTES 0x00700000

#define IMAGE_SCN_MEM_DISCARDABLE 0x02000000
#define IMAGE_SCN_MEM_NOT_CACHED 0x04000000
#define IMAGE_SCN_MEM_NOT_PAGED 0x08000000

#define IMAGE_SCN_MEM_SHARED 0x10000000
#define IMAGE_SCN_MEM_EXECUTE 0x20000000
#define IMAGE_SCN_MEM_READ 0x40000000
#define IMAGE_SCN_MEM_WRITE 0x80000000

/*
 * Base Relocation
 */
typedef struct basereloc_header
{
    br_uint_32 rva;
    br_uint_32 size;
} basereloc_header;

#define IMAGE_REL_BASED_ABSOLUTE 0
#define IMAGE_REL_BASED_HIGH 1
#define IMAGE_REL_BASED_LOW 2
#define IMAGE_REL_BASED_HIGHLOW 3
#define IMAGE_REL_BASED_HIGHADJ 4
#define IMAGE_REL_BASED_MIPS_JMPADDR 5

#define ENTRY_TYPE(e) ((e) >> 12)
#define ENTRY_OFFSET(e) ((e)&0xfff)

/*
 * Exports
 */
typedef struct export_directory
{
    br_uint_32 flags;
    br_uint_32 timestamp;
    br_uint_16 major_version;
    br_uint_16 minor_version;
    br_uint_32 name;
    br_uint_32 ordinal_base;
    br_uint_32 n_entries;
    br_uint_32 n_names;
    br_uint_32 export_table;
    br_uint_32 name_table;
    br_uint_32 ordinal_table;
} export_directory;

/*
 * Imports
 */
typedef struct import_directory
{
    br_uint_32 lookup_table;
    br_uint_32 timestamp;
    br_uint_32 forwarder;
    br_uint_32 name;
    br_uint_32 address_table;
} import_directory;
