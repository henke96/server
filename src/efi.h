_Static_assert(!hc_ILP32, "Pointers not 64 bit");

#define efi_guid_GRAPHICS_OUTPUT_PROTOCOL { \
    0x9042a9de, 0x23dc, 0x4a38, { 0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a } \
}

struct efi_guid {
    uint32_t data1;
    uint16_t data2;
    uint16_t data3;
    uint8_t data4[8];
};

struct efi_tableHeader {
    uint64_t signature;
    uint32_t revision;
    uint32_t headerSize;
    uint32_t crc32;
    uint32_t reserved;
};

struct efi_time {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t __pad;
    uint32_t nanosecond;
    int16_t timezone;
    uint8_t daylight;
    uint8_t __pad2;
};

struct efi_timeCapabilities {
    uint32_t resolution;
    uint32_t accuracy;
    uint8_t setsToZero;
    char __pad[3];
};

struct efi_inputKey {
    uint16_t scanCode;
    uint16_t unicodeChar;
};

struct efi_simpleTextInputProtocol {
    int64_t (hc_MS_ABI *reset)(struct efi_simpleTextInputProtocol *self, uint8_t extendedVerification);
    int64_t (hc_MS_ABI *readKeyStroke)(struct efi_simpleTextInputProtocol *self, struct efi_inputKey *key);
    void *waitForKeyEvent;
};

struct efi_simpleTextOutputMode {
    int32_t maxMode;
    int32_t mode;
    int32_t attribute;
    int32_t cursorColumn;
    int32_t cursorRow;
    uint8_t cursorVisible;
    char __pad[3];
};

struct efi_simpleTextOutputProtocol {
    int64_t (hc_MS_ABI *reset)(struct efi_simpleTextOutputProtocol *self, uint8_t extendedVerification);
    int64_t (hc_MS_ABI *outputString)(struct efi_simpleTextOutputProtocol *self, const uint16_t *string);
    int64_t (hc_MS_ABI *testString)(struct efi_simpleTextOutputProtocol *self, const uint16_t *string);
    int64_t (hc_MS_ABI *queryMode)(struct efi_simpleTextOutputProtocol *self, uint64_t modeNumber, uint64_t *columns, uint64_t *rows);
    int64_t (hc_MS_ABI *setMode)(struct efi_simpleTextOutputProtocol *self, uint64_t modeNumber);
    int64_t (hc_MS_ABI *setAttribute)(struct efi_simpleTextOutputProtocol *self, uint64_t attribute);
    int64_t (hc_MS_ABI *clearScreen)(struct efi_simpleTextOutputProtocol *self);
    int64_t (hc_MS_ABI *setCursorPosition)(struct efi_simpleTextOutputProtocol *self, uint64_t column, uint64_t row);
    int64_t (hc_MS_ABI *enableCursor)(struct efi_simpleTextOutputProtocol *self, uint8_t visible);
    struct efi_simpleTextOutputMode *mode;
};

enum efi_memoryType {
    efi_RESERVED_MEMORY_TYPE,
    efi_LOADER_CODE,
    efi_LOADER_DATA,
    efi_BOOT_SERVICES_CODE,
    efi_BOOT_SERVICES_DATA,
    efi_RUNTIME_SERVICES_CODE,
    efi_RUNTIME_SERVICES_DATA,
    efi_CONVENTIONAL_MEMORY,
    efi_UNUSABLE_MEMORY,
    efi_ACPI_RECLAIM_MEMORY,
    efi_ACPI_MEMORY_NVS,
    efi_MEMORY_MAPPED_IO,
    efi_MEMORY_MAPPED_IO_PORT_SPACE,
    efi_PAL_CODE,
    efi_PERSISTENT_MEMORY,
    efi_MAX_MEMORY_TYPE
};

struct efi_memoryDescriptor {
    enum efi_memoryType type;
    uint32_t __pad;
    uint64_t physicalStart;
    uint64_t virtualStart;
    uint64_t numberOfPages;
    uint64_t attribute;
};

enum efi_resetType {
    efi_RESET_COLD,
    efi_RESET_WARM,
    efi_RESET_SHUTDOWN,
    efi_RESET_PLATFORM_SPECIFIC
};

struct efi_capsuleHeader {
    struct efi_guid capsuleGuid;
    uint32_t headerSize;
    uint32_t flags;
    uint32_t capsuleImageSize;
};

struct efi_runtimeServices {
    struct efi_tableHeader header;
    int64_t (hc_MS_ABI *getTime)(struct efi_time *time, struct efi_timeCapabilities *capabilities);
    int64_t (hc_MS_ABI *setTime)(struct efi_time *time);
    int64_t (hc_MS_ABI *getWakeupTime)(uint8_t *enabled, uint8_t *pending, struct efi_time *time);
    int64_t (hc_MS_ABI *setWakeupTime)(uint8_t enable, struct efi_time *time);
    int64_t (hc_MS_ABI *setVirtualAddressMap)(
        uint64_t memoryMapSize,
        uint64_t descriptorSize,
        uint32_t descriptorVersion,
        struct efi_memoryDescriptor *virtualMap
    );
    int64_t (hc_MS_ABI *convertPointer)(uint64_t debugDisposition, void **address);
    int64_t (hc_MS_ABI *getVariable)(
        uint16_t *variableName,
        struct efi_guid *vendorGuid,
        uint32_t *attributes,
        uint64_t *dataSize,
        void *data
    );
    int64_t (hc_MS_ABI *getNextVariableName)(
        uint64_t *variableNameSize,
        uint16_t *variableName,
        struct efi_guid *vendorGuid
    );
    int64_t (hc_MS_ABI *setVariable)(
        uint16_t *variableName,
        struct efi_guid *vendorGuid,
        uint32_t attributes,
        uint64_t dataSize,
        void *data
    );
    int64_t (hc_MS_ABI *getNextHighMonotonicCount)(uint32_t *highCount);
    void (hc_MS_ABI *resetSystem)(
        enum efi_resetType type,
        int64_t resetStatus,
        uint64_t dataSize,
        void *resetData
    );
    int64_t (hc_MS_ABI *updateCapsule)(
        struct efi_capsuleHeader **capsuleHeaderArray,
        uint64_t capsuleCount,
        uint64_t scatterGatherList
    );
    int64_t (hc_MS_ABI *queryCapsuleCapabilities)(
        struct efi_capsuleHeader **capsuleHeaderArray,
        uint64_t capsuleCount,
        uint64_t *maximumCapsuleSize,
        enum efi_resetType *resetType
    );
    int64_t (hc_MS_ABI *queryVariableInfo)(
        uint32_t attributes,
        uint64_t *maximumVariableStorageSize,
        uint64_t *remainingVariableStorageSize,
        uint64_t *maximumVariableSize
    );
};

enum efi_allocateType {
    efi_ALLOCATE_ANY_PAGES,
    efi_ALLOCATE_MAX_ADDRESS,
    efi_ALLOCATE_ADDRESS,
    efi_MAX_ALLOCATE_TYPE
};

enum efi_timerDelay {
    efi_TIMER_CANCEL,
    efi_TIMER_PERIODIC,
    efi_TIMER_RELATIVE
};

enum efi_interfaceType {
    efi_NATIVE_INTERFACE
};

enum efi_locateSearchType {
    efi_ALL_HANDLES,
    efi_BY_REGISTER_NOTIFY,
    efi_BY_PROTOCOL
};

struct efi_devicePathProtocol {
    uint8_t type;
    uint8_t subType;
    uint8_t length[2];
};

struct efi_openProtocolInformationEntry {
    void *agentHandle;
    void *controllerHandle;
    uint32_t attributes;
    uint32_t openCount;
};

struct efi_bootServices {
    struct efi_tableHeader header;
    uint64_t (hc_MS_ABI *raiseTpl)(uint64_t newTpl);
    void (hc_MS_ABI *restoreTpl)(uint64_t oldTpl);
    int64_t (hc_MS_ABI *allocatePages)(
        enum efi_allocateType type,
        enum efi_memoryType memoryType,
        uint64_t pages,
        uint64_t *memoryAddress
    );
    int64_t (hc_MS_ABI *freePages)(uint64_t memoryAddress, uint64_t pages);
    int64_t (hc_MS_ABI *getMemoryMap)(
        uint64_t *memoryMapSize,
        void *memoryMap,
        uint64_t *mapKey,
        uint64_t *descriptorSize,
        uint32_t *descriptorVersion
    );
    int64_t (hc_MS_ABI *allocatePool)(enum efi_memoryType type, uint64_t size, void **buffer);
    int64_t (hc_MS_ABI *freePool)(void *buffer);
    int64_t (hc_MS_ABI *createEvent)(
        uint32_t type,
        uint64_t notifyTpl,
        void (hc_MS_ABI *notifyFunction)(void *event, void *context),
        void *notifyContext,
        void *event
    );
    int64_t (hc_MS_ABI *setTimer)(void *event, enum efi_timerDelay type, uint64_t triggerTime);
    int64_t (hc_MS_ABI *waitForEvent)(uint64_t numberOfEvents, void **event, uint64_t *index);
    int64_t (hc_MS_ABI *signalEvent)(void *event);
    int64_t (hc_MS_ABI *closeEvent)(void *event);
    int64_t (hc_MS_ABI *checkEvent)(void *event);
    int64_t (hc_MS_ABI *installProtocolInterface)(
        void *handle,
        struct efi_guid *protocol,
        enum efi_interfaceType interfaceType,
        void *interface
    );
    int64_t (hc_MS_ABI *reinstallProtocolInterface)(
        void *handle,
        struct efi_guid *protocol,
        void *oldInterface,
        void *newInterface
    );
    int64_t (hc_MS_ABI *uninstallProtocolInterface)(void *handle, struct efi_guid *protocol, void *interface);
    int64_t (hc_MS_ABI *handleProtocol)(void *handle, struct efi_guid *protocol, void **interface);
    void *reserved;
    int64_t (hc_MS_ABI *registerProtocolNotify)(struct efi_guid *protocol, void *event, void **registration);
    int64_t (hc_MS_ABI *locateHandle)(
        enum efi_locateSearchType searchType,
        struct efi_guid *protocol,
        void *searchKey,
        uint64_t *bufferSize,
        void **bufferHandle
    );
    int64_t (hc_MS_ABI *locateDevicePath)(
        struct efi_guid *protocol,
        struct efi_devicePathProtocol **devicePath,
        void **deviceHandle
    );
    int64_t (hc_MS_ABI *installConfigurationTable)(struct efi_guid *guid, void *table);
    int64_t (hc_MS_ABI *loadImage)(
        uint8_t bootPolicy,
        void *parentImageHandle,
        struct efi_devicePathProtocol *devicePath,
        void *sourceBuffer,
        uint64_t sourceSize,
        void **imageHandle
    );
    int64_t (hc_MS_ABI *startImage)(void *imageHandle, uint64_t *exitDataSize, uint16_t **exitData);
    int64_t (hc_MS_ABI *exit)(void *imageHandle, int64_t exitStatus, uint64_t exitDataSize, uint16_t *exitData);
    int64_t (hc_MS_ABI *unloadImage)(void *imageHandle);
    int64_t (hc_MS_ABI *exitBootServices)(void *imageHandle, uint64_t mapKey);
    int64_t (hc_MS_ABI *getNextMonotonicCount)(uint64_t *count);
    int64_t (hc_MS_ABI *stall)(uint64_t microseconds);
    int64_t (hc_MS_ABI *setWatchdogTimer)(
        uint64_t timeout,
        uint64_t watchdogCode,
        uint64_t dataSize,
        uint16_t *watchdogData
    );
    int64_t (hc_MS_ABI *connectController)(
        void *controllerHandle,
        void **driverImageHandle,
        struct efi_devicePathProtocol *remainingDevicePath,
        uint8_t recursive
    );
    int64_t (hc_MS_ABI *disconnectController)(void *controllerHandle, void *driverImageHandle, void *childHandle);
    int64_t (hc_MS_ABI *openProtocol)(
        void *handle,
        struct efi_guid *protocol,
        void **interface,
        void *agentHandle,
        void *controllerHandle,
        uint32_t attributes
    );
    int64_t (hc_MS_ABI *closeProtocol)(
        void *handle,
        struct efi_guid *protocol,
        void *agentHandle,
        void *controllerHandle
    );
    int64_t (hc_MS_ABI *openProtocolInformation)(
        void *handle,
        struct efi_guid *protocol,
        struct efi_openProtocolInformationEntry **entryBuffer,
        uint64_t *entryCount
    );
    int64_t (hc_MS_ABI *protocolsPerHandle)(void *handle, struct efi_guid ***protocolBuffer, uint64_t *protocolBufferCount);
    int64_t (hc_MS_ABI *locateHandleBuffer)(
        enum efi_locateSearchType searchType,
        struct efi_guid *protocol,
        void *searchKey,
        uint64_t *noHandles,
        void ***handleBuffer
    );
    int64_t (hc_MS_ABI *locateProtocol)(struct efi_guid *protocol, void *registration, void **interface);
    int64_t (hc_MS_ABI *installMultipleProtocolInterfaces)(void **handle, ...);
    int64_t (hc_MS_ABI *uninstallMultipleProtocolInterfaces)(void *handle, ...);
    int64_t (hc_MS_ABI *calculateCrc32)(void *data, uint64_t dataSize, uint32_t *crc32);
    void (hc_MS_ABI *copyMem)(void *destination, void *source, uint64_t length);
    void (hc_MS_ABI *setMem)(void *buffer, uint64_t size, uint8_t value);
    int64_t (hc_MS_ABI *createEventEx)(
        uint32_t type,
        uint64_t notifyTpl,
        void (hc_MS_ABI *notifyFunction)(void *event, void *context),
        const void *notifyContext,
        const struct efi_guid *eventGroup,
        void **event
    );
};

struct efi_configurationTable {
    struct efi_guid vendorGuid;
    void *vendorTable;
};

struct efi_systemTable {
    struct efi_tableHeader header;
    uint16_t *vendor;
    uint32_t revision;
    int32_t __pad;
    void *consoleInHandle;
    struct efi_simpleTextInputProtocol *consoleIn;
    void *consoleOutHandle;
    struct efi_simpleTextOutputProtocol *consoleOut;
    void *standardErrorHandle;
    struct efi_simpleTextOutputProtocol *standardError;
    struct efi_runtimeServices *runtimeServices;
    struct efi_bootServices *bootServices;
    uint64_t numberOfTableEntries;
    struct efi_configurationTable *configurationTable;
};

// Graphics output protocol
enum efi_graphicsPixelFormat {
    efi_PIXEL_RED_GREEN_BLUE_RESERVED_8BIT_PER_COLOR,
    efi_PIXEL_BLUE_GREEN_RED_RESERVED_8BIT_PER_COLOR,
    efi_PIXEL_BIT_MASK,
    efi_PIXEL_BLT_ONLY,
    efi_PIXEL_FORMAT_MAX
};

struct efi_pixelBitmask {
    uint32_t redMask;
    uint32_t greenMask;
    uint32_t blueMask;
    uint32_t reservedMask;
};

struct efi_graphicsOutputModeInformation {
    uint32_t version;
    uint32_t horizontalResolution;
    uint32_t verticalResolution;
    enum efi_graphicsPixelFormat pixelFormat;
    struct efi_pixelBitmask pixelInformation;
    uint32_t pixelsPerScanLine;
};

enum efi_graphicsOutputBltOperation {
    efi_BLT_VIDEO_FILL,
    efi_BLT_VIDEO_TO_BLT_BUFFER,
    efi_BLT_BUFFER_TO_VIDEO,
    efi_BLT_VIDEO_TO_VIDEO,
    efi_GRAPHICS_OUTPUT_BLT_OPERATION_MAX
};

struct efi_graphicsOutputBltPixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;
};

struct efi_graphicsOutputProtocolMode {
    uint32_t maxMode;
    uint32_t mode;
    struct efi_graphicsOutputModeInformation *info;
    uint64_t sizeOfInfo;
    uint64_t frameBufferBase;
    uint64_t frameBufferSize;
};

struct efi_graphicsOutputProtocol {
    int64_t (hc_MS_ABI *queryMode)(
        struct efi_graphicsOutputProtocol *self,
        uint32_t modeNumber,
        uint64_t *sizeOfInfo,
        struct efi_graphicsOutputModeInformation **info
    );
    int64_t (hc_MS_ABI *setMode)(
        struct efi_graphicsOutputProtocol *self,
        uint32_t modeNumber
    );
    int64_t (hc_MS_ABI *blt)(
        struct efi_graphicsOutputProtocol *self,
        struct efi_graphicsOutputBltPixel *bltBuffer,
        enum efi_graphicsOutputBltOperation bltOperation,
        uint64_t sourceX,
        uint64_t sourceY,
        uint64_t destinationX,
        uint64_t destinationY,
        uint64_t width,
        uint64_t height,
        uint64_t delta
    );
    struct efi_graphicsOutputProtocolMode *mode;
};
