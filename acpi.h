typedef struct _RSDP{
    char signature[8];
    unsigned char checksum;
    char oem_id[6];
    unsigned char revision;
    unsigned int rsdt_address;
    unsigned int length;
    unsigned long long xsdt_address;
    unsigned char extended_checksum;
    char reserved[3];
}RSDP;

typedef struct _XSDP{
    char signatre[4];
    unsigned int length;
    unsigned char revision;
    unsigned char checksum;
    char oem_id[6];
    
}XSDP;