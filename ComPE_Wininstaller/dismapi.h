/****************************************************************************\

    DismApi.H

    Copyright (c) Microsoft Corporation.
    All rights reserved.

\****************************************************************************/

#ifndef _DISMAPI_H_
#define _DISMAPI_H_

#include <winapifamily.h>

#pragma region Desktop Family or DISM Package
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP | WINAPI_PARTITION_PKG_DISM)

#ifdef __cplusplus
extern "C"
{
#endif

#if (NTDDI_VERSION < NTDDI_WIN8) && !defined(DISMAPI_LATEST)
#error DISMAPI is only supported in Windows 8 and later
#endif

    //////////////////////////////////////////////////////////////////////////////
    //
    // Typedefs
    //
    //////////////////////////////////////////////////////////////////////////////

    typedef UINT DismSession;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Callbacks
    //
    //////////////////////////////////////////////////////////////////////////////

    typedef void(CALLBACK *DISM_PROGRESS_CALLBACK)(_In_ UINT Current, _In_ UINT Total, _In_opt_ PVOID UserData);

    //////////////////////////////////////////////////////////////////////////////
    //
    // Constants
    //
    //////////////////////////////////////////////////////////////////////////////

#define DISM_ONLINE_IMAGE L"DISM_{53BFAE52-B167-4E2F-A258-0A37B57FF845}"
#define DISM_SESSION_DEFAULT 0

// Mount flags
#define DISM_MOUNT_READWRITE 0x00000000
#define DISM_MOUNT_READONLY 0x00000001
#define DISM_MOUNT_OPTIMIZE 0x00000002
#define DISM_MOUNT_CHECK_INTEGRITY 0x00000004
#if (NTDDI_VERSION >= NTDDI_WIN10_MN || defined (DISMAPI_LATEST))
#define DISM_MOUNT_SUPPORT_EA 0x00000008
#endif // (NTDDI_VERSION >= NTDDI_WIN10_MN || defined (DISMAPI_LATEST))

// Unmount flags
#define DISM_COMMIT_IMAGE 0x00000000
#define DISM_DISCARD_IMAGE 0x00000001

// Commit flags
#define DISM_COMMIT_GENERATE_INTEGRITY 0x00010000
#define DISM_COMMIT_APPEND 0x00020000
#if (NTDDI_VERSION >= NTDDI_WIN10_MN || defined (DISMAPI_LATEST))
#define DISM_COMMIT_SUPPORT_EA 0x00040000
#endif // (NTDDI_VERSION >= NTDDI_WIN10_MN || defined (DISMAPI_LATEST))

// Commit flags may also be used with unmount.  AND this with unmount flags and you will
// get the commit-specific flags.
#define DISM_COMMIT_MASK 0xffff0000

// Reserved storage state flags
#if (NTDDI_VERSION >= NTDDI_WIN10_VB || defined (DISMAPI_LATEST))
#define DISM_RESERVED_STORAGE_DISABLED 0x00000000
#define DISM_RESERVED_STORAGE_ENABLED 0x00000001
#endif // (NTDDI_VERSION >= NTDDI_WIN10_VB || defined (DISMAPI_LATEST))

    //////////////////////////////////////////////////////////////////////////////
    //
    // Enums
    //
    //////////////////////////////////////////////////////////////////////////////

    typedef enum _DismLogLevel
    {
        DismLogErrors = 0,
        DismLogErrorsWarnings,
        DismLogErrorsWarningsInfo,
#if (NTDDI_VERSION >= NTDDI_WIN10_MN || defined (DISMAPI_LATEST))
        DismLogErrorsWarningsInfoDebug
#endif // (NTDDI_VERSION >= NTDDI_WIN10_MN || defined (DISMAPI_LATEST))
    } DismLogLevel;

    typedef enum _DismImageIdentifier
    {
        DismImageIndex = 0,
        DismImageName,
#if (NTDDI_VERSION >= NTDDI_WIN10_CU || defined (DISMAPI_LATEST))
        DismImageNone,
#endif // (NTDDI_VERSION >= NTDDI_WIN10_CU || defined (DISMAPI_LATEST))
    } DismImageIdentifier;

    typedef enum _DismMountMode
    {
        DismReadWrite = 0,
        DismReadOnly
    } DismMountMode;

    typedef enum _DismImageType
    {
        DismImageTypeUnsupported = -1,
        DismImageTypeWim = 0,
        DismImageTypeVhd = 1
    } DismImageType;

    typedef enum _DismImageBootable
    {
        DismImageBootableYes = 0,
        DismImageBootableNo,
        DismImageBootableUnknown
    } DismImageBootable;

    typedef enum _DismMountStatus
    {
        DismMountStatusOk = 0,
        DismMountStatusNeedsRemount,
        DismMountStatusInvalid
    } DismMountStatus;

    typedef enum _DismImageHealthState
    {
        DismImageHealthy = 0,
        DismImageRepairable,
        DismImageNonRepairable
    } DismImageHealthState;

    typedef enum _DismPackageIdentifier
    {
        DismPackageNone = 0,
        DismPackageName,
        DismPackagePath
    } DismPackageIdentifier;

    typedef enum _DismPackageFeatureState
    {
        DismStateNotPresent = 0,
        DismStateUninstallPending,
        DismStateStaged,
        DismStateResolved, // For internal use only
        DismStateRemoved = DismStateResolved,
        DismStateInstalled,
        DismStateInstallPending,
        DismStateSuperseded,
        DismStatePartiallyInstalled
    } DismPackageFeatureState;

    typedef enum _DismReleaseType
    {
        DismReleaseTypeCriticalUpdate = 0,
        DismReleaseTypeDriver,
        DismReleaseTypeFeaturePack,
        DismReleaseTypeHotfix,
        DismReleaseTypeSecurityUpdate,
        DismReleaseTypeSoftwareUpdate,
        DismReleaseTypeUpdate,
        DismReleaseTypeUpdateRollup,
        DismReleaseTypeLanguagePack,
        DismReleaseTypeFoundation,
        DismReleaseTypeServicePack,
        DismReleaseTypeProduct,
        DismReleaseTypeLocalPack,
        DismReleaseTypeOther,
#if (NTDDI_VERSION >= NTDDI_WIN10_RS2 || defined (DISMAPI_LATEST))
        DismReleaseTypeOnDemandPack
#endif // (NTDDI_VERSION >= NTDDI_WIN10_RS2 || defined (DISMAPI_LATEST))
    } DismReleaseType;

    typedef enum _DismRestartType
    {
        DismRestartNo = 0,
        DismRestartPossible,
        DismRestartRequired
    } DismRestartType;

    typedef enum _DismDriverSignature
    {
        DismDriverSignatureUnknown = 0,
        DismDriverSignatureUnsigned = 1,
        DismDriverSignatureSigned = 2
    } DismDriverSignature;

    typedef enum _DismFullyOfflineInstallableType
    {
        DismFullyOfflineInstallable = 0,
        DismFullyOfflineNotInstallable,
        DismFullyOfflineInstallableUndetermined
    } DismFullyOfflineInstallableType;

#if (NTDDI_VERSION >= NTDDI_WIN10_VB || defined (DISMAPI_LATEST))
    // Keep in-sync with StubPackageOption defined in onecore\base\ntsetup\opktools\dism\idl\AppxProviderInterfaces.idl.
    // Need to redefine it here because the consumers of this header cannot import dismcore.tlb, where all DISM IDLs are
    // built into.
    typedef enum _DismStubPackageOption
    {
        DismStubPackageOptionNone = 0,
        DismStubPackageOptionInstallFull = 1,
        DismStubPackageOptionInstallStub = 2,
    } DismStubPackageOption;
#endif // (NTDDI_VERSION >= NTDDI_WIN10_VB || defined (DISMAPI_LATEST))

//////////////////////////////////////////////////////////////////////////////
//
// Structs
//
//////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)

    typedef struct _DismPackage
    {
        PCWSTR PackageName;
        DismPackageFeatureState PackageState;
        DismReleaseType ReleaseType;
        SYSTEMTIME InstallTime;
    } DismPackage;

    typedef struct _DismCustomProperty
    {
        PCWSTR Name;
        PCWSTR Value;
        PCWSTR Path;
    } DismCustomProperty;

    typedef struct _DismFeature
    {
        PCWSTR FeatureName;
        DismPackageFeatureState State;
    } DismFeature;

#if (NTDDI_VERSION >= NTDDI_WIN10 || defined (DISMAPI_LATEST))
    typedef struct _DismCapability
    {
        PCWSTR Name;
        DismPackageFeatureState State;
    } DismCapability;
#endif // (NTDDI_VERSION >= NTDDI_WIN10 || defined (DISMAPI_LATEST))

    typedef struct _DismPackageInfo
    {
        PCWSTR PackageName;
        DismPackageFeatureState PackageState;
        DismReleaseType ReleaseType;
        SYSTEMTIME InstallTime;
        BOOL Applicable;
        PCWSTR Copyright;
        PCWSTR Company;
        SYSTEMTIME CreationTime;
        PCWSTR DisplayName;
        PCWSTR Description;
        PCWSTR InstallClient;
        PCWSTR InstallPackageName;
        SYSTEMTIME LastUpdateTime;
        PCWSTR ProductName;
        PCWSTR ProductVersion;
        DismRestartType RestartRequired;
        DismFullyOfflineInstallableType FullyOffline;
        PCWSTR SupportInformation;
        DismCustomProperty *CustomProperty;
        UINT CustomPropertyCount;
        DismFeature *Feature;
        UINT FeatureCount;
    } DismPackageInfo;

    typedef struct _DismFeatureInfo
    {
        PCWSTR FeatureName;
        DismPackageFeatureState FeatureState;
        PCWSTR DisplayName;
        PCWSTR Description;
        DismRestartType RestartRequired;
        DismCustomProperty *CustomProperty;
        UINT CustomPropertyCount;
    } DismFeatureInfo;

#if (NTDDI_VERSION >= NTDDI_WIN10 || defined (DISMAPI_LATEST))
    typedef struct _DismCapabilityInfo
    {
        PCWSTR Name;
        DismPackageFeatureState State;
        PCWSTR DisplayName;
        PCWSTR Description;
        DWORD DownloadSize;
        DWORD InstallSize;
    } DismCapabilityInfo;
#endif // (NTDDI_VERSION >= NTDDI_WIN10 || defined (DISMAPI_LATEST))

    typedef struct _DismString
    {
        PCWSTR Value;
    } DismString;

    typedef DismString DismLanguage;

    typedef struct _DismWimCustomizedInfo
    {
        UINT Size;
        UINT DirectoryCount;
        UINT FileCount;
        SYSTEMTIME CreatedTime;
        SYSTEMTIME ModifiedTime;
    } DismWimCustomizedInfo;

    typedef struct _DismImageInfo
    {
        DismImageType ImageType;
        UINT ImageIndex;
        PCWSTR ImageName;
        PCWSTR ImageDescription;
        UINT64 ImageSize;
        UINT Architecture;
        PCWSTR ProductName;
        PCWSTR EditionId;
        PCWSTR InstallationType;
        PCWSTR Hal;
        PCWSTR ProductType;
        PCWSTR ProductSuite;
        UINT MajorVersion;
        UINT MinorVersion;
        UINT Build;
        UINT SpBuild;
        UINT SpLevel;
        DismImageBootable Bootable;
        PCWSTR SystemRoot;
        DismLanguage *Language;
        UINT LanguageCount;
        UINT DefaultLanguageIndex;
        VOID *CustomizedInfo;
    } DismImageInfo;

    typedef struct _DismMountedImageInfo
    {
        PCWSTR MountPath;
        PCWSTR ImageFilePath;
        UINT ImageIndex;
        DismMountMode MountMode;
        DismMountStatus MountStatus;
    } DismMountedImageInfo;

    typedef struct _DismDriverPackage
    {
        PCWSTR PublishedName;
        PCWSTR OriginalFileName;
        BOOL InBox;
        PCWSTR CatalogFile;
        PCWSTR ClassName;
        PCWSTR ClassGuid;
        PCWSTR ClassDescription;
        BOOL BootCritical;
        DismDriverSignature DriverSignature;
        PCWSTR ProviderName;
        SYSTEMTIME Date;
        UINT MajorVersion;
        UINT MinorVersion;
        UINT Build;
        UINT Revision;
    } DismDriverPackage;

    typedef struct _DismDriver
    {
        PCWSTR ManufacturerName;
        PCWSTR HardwareDescription;
        PCWSTR HardwareId;
        UINT Architecture;
        PCWSTR ServiceName;
        PCWSTR CompatibleIds;
        PCWSTR ExcludeIds;
    } DismDriver;

#if (NTDDI_VERSION >= NTDDI_WIN10_VB || defined (DISMAPI_LATEST))
    typedef struct _DismAppxPackage
    {
        PCWSTR PackageName;
        PCWSTR DisplayName;
        PCWSTR PublisherId;
        UINT MajorVersion;
        UINT MinorVersion;
        UINT Build;
        UINT RevisionNumber;
        UINT Architecture;
        PCWSTR ResourceId;
        PCWSTR InstallLocation;
        _Maybenull_ PCWSTR Region;

    } DismAppxPackage;
#endif // (NTDDI_VERSION >= NTDDI_WIN10_VB || defined (DISMAPI_LATEST))

#pragma pack(pop)

    //////////////////////////////////////////////////////////////////////////////
    //
    // Functions
    //
    //////////////////////////////////////////////////////////////////////////////

    HRESULT WINAPI
    DismInitialize(
        _In_ DismLogLevel LogLevel,
        _In_opt_ PCWSTR LogFilePath,
        _In_opt_ PCWSTR ScratchDirectory);

    HRESULT WINAPI
    DismShutdown();

    HRESULT WINAPI
    DismMountImage(
        _In_ PCWSTR ImageFilePath,
        _In_ PCWSTR MountPath,
        _In_ UINT ImageIndex,
        _In_opt_ PCWSTR ImageName,
        _In_ DismImageIdentifier ImageIdentifier,
        _In_ DWORD Flags,
        _In_opt_ HANDLE CancelEvent,
        _In_opt_ DISM_PROGRESS_CALLBACK Progress,
        _In_opt_ PVOID UserData);

    HRESULT WINAPI
    DismUnmountImage(
        _In_ PCWSTR MountPath,
        _In_ DWORD Flags,
        _In_opt_ HANDLE CancelEvent,
        _In_opt_ DISM_PROGRESS_CALLBACK Progress,
        _In_opt_ PVOID UserData);

    HRESULT WINAPI
    DismOpenSession(
        _In_ PCWSTR ImagePath,
        _In_opt_ PCWSTR WindowsDirectory,
        _In_opt_ PCWSTR SystemDrive,
        _Out_ DismSession *Session);

    HRESULT WINAPI
    DismCloseSession(
        _In_ DismSession Session);

    HRESULT WINAPI
    DismGetLastErrorMessage(
        _Outptr_result_maybenull_ DismString **ErrorMessage);

    HRESULT WINAPI
    DismRemountImage(
        _In_ PCWSTR MountPath);

    HRESULT WINAPI
    DismCommitImage(
        _In_ DismSession Session,
        _In_ DWORD Flags,
        _In_opt_ HANDLE CancelEvent,
        _In_opt_ DISM_PROGRESS_CALLBACK Progress,
        _In_opt_ PVOID UserData);

    HRESULT WINAPI
    DismGetImageInfo(
        _In_ PCWSTR ImageFilePath,
        _Outptr_result_buffer_(*Count) DismImageInfo **ImageInfo,
        _Out_ UINT *Count);

    HRESULT WINAPI
    DismGetMountedImageInfo(
        _Outptr_result_buffer_(*Count) DismMountedImageInfo **MountedImageInfo,
        _Out_ UINT *Count);

    HRESULT WINAPI
    DismCleanupMountpoints();

    HRESULT WINAPI
    DismCheckImageHealth(
        _In_ DismSession Session,
        _In_ BOOL ScanImage,
        _In_opt_ HANDLE CancelEvent,
        _In_opt_ DISM_PROGRESS_CALLBACK Progress,
        _In_opt_ PVOID UserData,
        _Out_ DismImageHealthState *ImageHealth);

    HRESULT WINAPI
    DismRestoreImageHealth(
        _In_ DismSession Session,
        _In_reads_opt_(SourcePathCount) PCWSTR *SourcePaths,
        _In_opt_ UINT SourcePathCount,
        _In_ BOOL LimitAccess,
        _In_opt_ HANDLE CancelEvent,
        _In_opt_ DISM_PROGRESS_CALLBACK Progress,
        _In_opt_ PVOID UserData);

    HRESULT WINAPI
    DismDelete(
        _In_ VOID *DismStructure);

    HRESULT WINAPI
    DismAddPackage(
        _In_ DismSession Session,
        _In_ PCWSTR PackagePath,
        _In_ BOOL IgnoreCheck,
        _In_ BOOL PreventPending,
        _In_opt_ HANDLE CancelEvent,
        _In_opt_ DISM_PROGRESS_CALLBACK Progress,
        _In_opt_ PVOID UserData);

    HRESULT WINAPI
    DismRemovePackage(
        _In_ DismSession Session,
        _In_ PCWSTR Identifier,
        _In_ DismPackageIdentifier PackageIdentifier,
        _In_opt_ HANDLE CancelEvent,
        _In_opt_ DISM_PROGRESS_CALLBACK Progress,
        _In_opt_ PVOID UserData);

    HRESULT WINAPI
    DismEnableFeature(
        _In_ DismSession Session,
        _In_ PCWSTR FeatureName,
        _In_opt_ PCWSTR Identifier,
        _In_opt_ DismPackageIdentifier PackageIdentifier,
        _In_ BOOL LimitAccess,
        _In_reads_opt_(SourcePathCount) PCWSTR *SourcePaths,
        _In_opt_ UINT SourcePathCount,
        _In_ BOOL EnableAll,
        _In_opt_ HANDLE CancelEvent,
        _In_opt_ DISM_PROGRESS_CALLBACK Progress,
        _In_opt_ PVOID UserData);

    HRESULT WINAPI
    DismDisableFeature(
        _In_ DismSession Session,
        _In_ PCWSTR FeatureName,
        _In_opt_ PCWSTR PackageName,
        _In_ BOOL RemovePayload,
        _In_opt_ HANDLE CancelEvent,
        _In_opt_ DISM_PROGRESS_CALLBACK Progress,
        _In_opt_ PVOID UserData);

    HRESULT WINAPI
    DismGetPackages(
        _In_ DismSession Session,
        _Outptr_result_buffer_(*Count) DismPackage **Package,
        _Out_ UINT *Count);

    HRESULT WINAPI
    DismGetPackageInfo(
        _In_ DismSession Session,
        _In_ PCWSTR Identifier,
        _In_ DismPackageIdentifier PackageIdentifier,
        _Out_ DismPackageInfo **PackageInfo);

    HRESULT WINAPI
    DismGetFeatures(
        _In_ DismSession Session,
        _In_opt_ PCWSTR Identifier,
        _In_opt_ DismPackageIdentifier PackageIdentifier,
        _Outptr_result_buffer_(*Count) DismFeature **Feature,
        _Out_ UINT *Count);

    HRESULT WINAPI
    DismGetFeatureInfo(
        _In_ DismSession Session,
        _In_ PCWSTR FeatureName,
        _In_opt_ PCWSTR Identifier,
        _In_opt_ DismPackageIdentifier PackageIdentifier,
        _Out_ DismFeatureInfo **FeatureInfo);

    HRESULT WINAPI
    DismGetFeatureParent(
        _In_ DismSession Session,
        _In_ PCWSTR FeatureName,
        _In_opt_ PCWSTR Identifier,
        _In_opt_ DismPackageIdentifier PackageIdentifier,
        _Outptr_result_buffer_(*Count) DismFeature **Feature,
        _Out_ UINT *Count);

    HRESULT WINAPI
    DismApplyUnattend(
        _In_ DismSession Session,
        _In_ PCWSTR UnattendFile,
        _In_ BOOL SingleSession);

    HRESULT WINAPI
    DismAddDriver(
        _In_ DismSession Session,
        _In_ PCWSTR DriverPath,
        _In_ BOOL ForceUnsigned);

    HRESULT WINAPI
    DismRemoveDriver(
        _In_ DismSession Session,
        _In_ PCWSTR DriverPath);

    HRESULT WINAPI
    DismGetDrivers(
        _In_ DismSession Session,
        _In_ BOOL AllDrivers,
        _Outptr_result_buffer_(*Count) DismDriverPackage **DriverPackage,
        _Out_ UINT *Count);

    HRESULT WINAPI
    DismGetDriverInfo(
        _In_ DismSession Session,
        _In_ PCWSTR DriverPath,
        _Outptr_result_buffer_(*Count) DismDriver **Driver,
        _Out_ UINT *Count,
        _Out_opt_ DismDriverPackage **DriverPackage);

#if (NTDDI_VERSION >= NTDDI_WIN10 || defined (DISMAPI_LATEST))
    HRESULT WINAPI
    DismGetCapabilities(
        _In_ DismSession Session,
        _Outptr_result_buffer_(*Count) DismCapability **Capability,
        _Out_ UINT *Count);

    HRESULT WINAPI
    DismGetCapabilityInfo(
        _In_ DismSession Session,
        _In_ PCWSTR Name,
        _Out_ DismCapabilityInfo **Info);

    HRESULT WINAPI
    DismAddCapability(
        _In_ DismSession Session,
        _In_ PCWSTR Name,
        _In_ BOOL LimitAccess,
        _In_reads_opt_(SourcePathCount) PCWSTR *SourcePaths,
        _In_opt_ UINT SourcePathCount,
        _In_opt_ HANDLE CancelEvent,
        _In_opt_ DISM_PROGRESS_CALLBACK Progress,
        _In_opt_ PVOID UserData);

    HRESULT WINAPI
    DismRemoveCapability(
        _In_ DismSession Session,
        _In_ PCWSTR Name,
        _In_opt_ HANDLE CancelEvent,
        _In_opt_ DISM_PROGRESS_CALLBACK Progress,
        _In_opt_ PVOID UserData);
#endif // (NTDDI_VERSION >= NTDDI_WIN10 || defined (DISMAPI_LATEST)) 

#if (NTDDI_VERSION >= NTDDI_WIN10_VB || defined (DISMAPI_LATEST))
    HRESULT WINAPI
    DismGetReservedStorageState(
        _In_ DismSession Session,
        _Out_ PDWORD State);

    HRESULT WINAPI
    DismSetReservedStorageState(
        _In_ DismSession Session,
        _In_ DWORD State);

    HRESULT WINAPI
    _DismGetProvisionedAppxPackages(
        DismSession Session,
        _Outptr_result_buffer_(*Count) DismAppxPackage **Package,
        _Out_ UINT *Count);

    HRESULT WINAPI
    _DismAddProvisionedAppxPackage(
        _In_ DismSession Session,
        _In_ PCWSTR AppPath,
        _In_reads_opt_(DependencyPackageCount) PCWSTR DependencyPackages[],
        _In_ UINT DependencyPackageCount,
        _In_reads_opt_(OptionalPackageCount) PCWSTR OptionalPackages[],
        _In_ UINT OptionalPackageCount,
        _In_reads_opt_(LicensePathCount) PCWSTR LicensePaths[],
        _In_ UINT LicensePathCount,
        _In_ BOOL SkipLicense,
        _In_opt_ PCWSTR CustomDataPath,
        _In_opt_ PCWSTR Region,
        _In_ DismStubPackageOption stubPackageOption);

    HRESULT WINAPI
    _DismRemoveProvisionedAppxPackage(
        _In_ DismSession Session,
        _In_ PCWSTR PackageName);
#endif // (NTDDI_VERSION >= NTDDI_WIN10_VB || defined (DISMAPI_LATEST))

#if (NTDDI_VERSION >= NTDDI_WIN10_FE || defined (DISMAPI_LATEST))
#define DismGetProvisionedAppxPackages _DismGetProvisionedAppxPackages
#define DismAddProvisionedAppxPackage _DismAddProvisionedAppxPackage
#define DismRemoveProvisionedAppxPackage _DismRemoveProvisionedAppxPackage
#endif // (NTDDI_VERSION >= NTDDI_WIN10_FE || defined (DISMAPI_LATEST))

#if (NTDDI_VERSION >= NTDDI_WIN10_CU || defined (DISMAPI_LATEST))
    HRESULT WINAPI
    DismAddLanguage(
        _In_ DismSession Session,
        _In_ PCWSTR LanguageName,
        _In_ BOOL PreventPending,
        _In_ BOOL LimitAccess,
        _In_reads_(SourcePathCount) PCWSTR SourcePaths[],
        _In_ UINT SourcePathCount,
        _In_opt_ HANDLE CancelEvent,
        _In_opt_ DISM_PROGRESS_CALLBACK Progress,
        _In_opt_ PVOID UserData);

    HRESULT WINAPI
    DismRemoveLanguage(
        _In_ DismSession Session,
        _In_ PCWSTR LanguageName,
        _In_opt_ HANDLE CancelEvent,
        _In_opt_ DISM_PROGRESS_CALLBACK Progress,
        _In_opt_ PVOID UserData);
#endif // (NTDDI_VERSION >= NTDDI_WIN10_CU || defined (DISMAPI_LATEST))

//////////////////////////////////////////////////////////////////////////////
//
// Success Codes
//
//////////////////////////////////////////////////////////////////////////////

// For online scenario, computer needs to be restarted when the return value is ERROR_SUCCESS_REBOOT_REQUIRED (3010L).

//
// MessageId: DISMAPI_S_RELOAD_IMAGE_SESSION_REQUIRED
//
// MessageText:
//
// The DISM session needs to be reloaded.
//
#define DISMAPI_S_RELOAD_IMAGE_SESSION_REQUIRED 0x00000001

//////////////////////////////////////////////////////////////////////////////
//
// Error Codes
//
//////////////////////////////////////////////////////////////////////////////

//
// MessageId: DISMAPI_E_DISMAPI_NOT_INITIALIZED
//
// MessageText:
//
// DISM API was not initialized for this process
//
#define DISMAPI_E_DISMAPI_NOT_INITIALIZED 0xC0040001

//
// MessageId: DISMAPI_E_SHUTDOWN_IN_PROGRESS
//
// MessageText:
//
// A DismSession was being shutdown when another operation was called on it
//
#define DISMAPI_E_SHUTDOWN_IN_PROGRESS 0xC0040002

//
// MessageId: DISMAPI_E_OPEN_SESSION_HANDLES
//
// MessageText:
//
// A DismShutdown was called while there were open DismSession handles
//
#define DISMAPI_E_OPEN_SESSION_HANDLES 0xC0040003

//
// MessageId: DISMAPI_E_INVALID_DISM_SESSION
//
// MessageText:
//
// An invalid DismSession handle was passed into a DISMAPI function
//
#define DISMAPI_E_INVALID_DISM_SESSION 0xC0040004

//
// MessageId: DISMAPI_E_INVALID_IMAGE_INDEX
//
// MessageText:
//
// An invalid image index was specified
//
#define DISMAPI_E_INVALID_IMAGE_INDEX 0xC0040005

//
// MessageId: DISMAPI_E_INVALID_IMAGE_NAME
//
// MessageText:
//
// An invalid image name was specified
//
#define DISMAPI_E_INVALID_IMAGE_NAME 0xC0040006

//
// MessageId: DISMAPI_E_UNABLE_TO_UNMOUNT_IMAGE_PATH
//
// MessageText:
//
// An image that is not a mounted WIM or mounted VHD was attempted to be unmounted
//
#define DISMAPI_E_UNABLE_TO_UNMOUNT_IMAGE_PATH 0xC0040007

//
// MessageId: DISMAPI_E_LOGGING_DISABLED
//
// MessageText:
//
// Failed to gain access to the log file user specified. Logging has been disabled..
//
#define DISMAPI_E_LOGGING_DISABLED 0xC0040009

//
// MessageId: DISMAPI_E_OPEN_HANDLES_UNABLE_TO_UNMOUNT_IMAGE_PATH
//
// MessageText:
//
// A DismSession with open handles was attempted to be unmounted
//
#define DISMAPI_E_OPEN_HANDLES_UNABLE_TO_UNMOUNT_IMAGE_PATH 0xC004000A

//
// MessageId: DISMAPI_E_OPEN_HANDLES_UNABLE_TO_MOUNT_IMAGE_PATH
//
// MessageText:
//
// A DismSession with open handles was attempted to be mounted
//
#define DISMAPI_E_OPEN_HANDLES_UNABLE_TO_MOUNT_IMAGE_PATH 0xC004000B

//
// MessageId: DISMAPI_E_OPEN_HANDLES_UNABLE_TO_REMOUNT_IMAGE_PATH
//
// MessageText:
//
// A DismSession with open handles was attempted to be remounted
//
#define DISMAPI_E_OPEN_HANDLES_UNABLE_TO_REMOUNT_IMAGE_PATH 0xC004000C

//
// MessageId: DISMAPI_E_PARENT_FEATURE_DISABLED
//
// MessageText:
//
// One or several parent features are disabled so current feature can not be enabled.
// Solutions:
// 1 Call function DismGetFeatureParent to get all parent features and enable all of them. Or
// 2 Set EnableAll to TRUE when calling function DismEnableFeature.
//
#define DISMAPI_E_PARENT_FEATURE_DISABLED 0xC004000D

//
// MessageId: DISMAPI_E_MUST_SPECIFY_ONLINE_IMAGE
//
// MessageText:
//
// The offline image specified is the running system. The macro DISM_ONLINE_IMAGE must be
// used instead.
//
#define DISMAPI_E_MUST_SPECIFY_ONLINE_IMAGE 0xC004000E

#if (NTDDI_VERSION >= NTDDI_WINBLUE || defined (DISMAPI_LATEST))
//
// MessageId: DISMAPI_E_INVALID_PRODUCT_KEY
//
// MessageText:
//
// The specified product key could not be validated. Check that the specified
// product key is valid and that it matches the target edition.
//
#define DISMAPI_E_INVALID_PRODUCT_KEY 0xC004000F
#endif // (NTDDI_VERSION >= NTDDI_WINBLUE || defined (DISMAPI_LATEST))

#if (NTDDI_VERSION >= NTDDI_WIN10_CU || defined (DISMAPI_LATEST))
//
// MessageId: DISMAPI_E_MUST_SPECIFY_INDEX_OR_NAME
//
// MessageText:
//
// An image file must be specified with either an index or a name.
//
#define DISMAPI_E_MUST_SPECIFY_INDEX_OR_NAME 0xC0040020
#endif // (NTDDI_VERSION >= NTDDI_WIN10_CU || defined (DISMAPI_LATEST))

//
// MessageId: DISMAPI_E_NEEDS_TO_REMOUNT_THE_IMAGE
//
// MessageText:
//
// The image needs to be remounted before any servicing operation.
//
#define DISMAPI_E_NEEDS_REMOUNT 0XC1510114

//
// MessageId: DISMAPI_E_UNKNOWN_FEATURE
//
// MessageText:
//
// The feature is not present in the package.
//
#define DISMAPI_E_UNKNOWN_FEATURE 0x800f080c

//
// MessageId: DISMAPI_E_BUSY
//
// MessageText:
//
// The current package and feature servicing infrastructure is busy.  Wait a
// bit and try the operation again.
//
#define DISMAPI_E_BUSY 0x800f0902

#ifdef __cplusplus
}
#endif

#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP | WINAPI_PARTITION_PKG_DISM) */
#pragma endregion

#endif // _DISMAPI_H_
