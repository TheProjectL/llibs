/* Include llibs.h header */
#include <llibs.h>
#include <windows.h>

/* Console */

/* Static variables */

static HANDLE   hcout   = NULL,
                hcin    = NULL,
                hcerr   = NULL;

lerr lcio_init(void) {
    hcout = GetStdHandle(STD_OUTPUT_HANDLE);
    hcin = GetStdHandle(STD_INPUT_HANDLE);
    hcerr = GetStdHandle(STD_ERROR_HANDLE);
    
    if (hcout == INVALID_HANDLE_VALUE
    || hcin == INVALID_HANDLE_VALUE
    || hcerr == INVALID_HANDLE_VALUE) {
        return L_ERROR;
    }

    return L_OK;
}


void lcout(const lc str[]) {
    WriteConsoleA(hcout, str, lssize(str), NULL, NULL);
}


void lcoutn(const lc str[], lsz len) {
    WriteConsoleA(hcout, str, lssizen(str, len), NULL, NULL);
}


void lcerr(const lc str[]) {
    WriteConsoleA(hcerr, str, lslen(str), NULL, NULL);
}


void lcerrn(const lc str[], lsz len) {
    WriteConsoleA(hcerr, str, lssizen(str, len), NULL, NULL);
}

void lcin(lc* dest, lc sep) {

}


void lcinn(lc* dest, lsz len) {
    ReadConsoleW(hcin, dest, len, NULL, NULL);
}

static const WORD color_text_lookup[] = {
    [(int)(LC_COLOR_BLACK)] = 0x00, [(int)(LC_COLOR_WHITE)] = 0x0F,
    
    [(int)(LC_COLOR_RED)] = FOREGROUND_RED | FOREGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_RED)] = FOREGROUND_RED,
    [(int)(LC_COLOR_BLUE)] = FOREGROUND_BLUE | FOREGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_BLUE)] = FOREGROUND_BLUE,
    [(int)(LC_COLOR_GREEN)] = FOREGROUND_GREEN | FOREGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_GREEN)] = FOREGROUND_GREEN,

    [(int)(LC_COLOR_GRAY)] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_GRAY)] = FOREGROUND_INTENSITY,

    [(int)(LC_COLOR_MAGNETA)] = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_MAGNETA)] = FOREGROUND_RED | FOREGROUND_BLUE,

    [(int)(LC_COLOR_YELLOW)] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_YELLOW)] = FOREGROUND_RED | FOREGROUND_GREEN,

    [(int)(LC_COLOR_CYAN)] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_CYAN)] = FOREGROUND_BLUE | FOREGROUND_GREEN,
};

static const WORD color_bg_lookup[] = {
    [(int)(LC_COLOR_BLACK)] = 0x00, [(int)(LC_COLOR_WHITE)] = 0x0F,
    
    [(int)(LC_COLOR_RED)] = BACKGROUND_RED | BACKGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_RED)] = BACKGROUND_RED,
    [(int)(LC_COLOR_BLUE)] = BACKGROUND_BLUE | BACKGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_BLUE)] = BACKGROUND_BLUE,
    [(int)(LC_COLOR_GREEN)] = BACKGROUND_GREEN | BACKGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_GREEN)] = BACKGROUND_GREEN,

    [(int)(LC_COLOR_GRAY)] = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_GRAY)] = BACKGROUND_INTENSITY,

    [(int)(LC_COLOR_MAGNETA)] = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_MAGNETA)] = BACKGROUND_RED | BACKGROUND_BLUE,

    [(int)(LC_COLOR_YELLOW)] = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_YELLOW)] = BACKGROUND_RED | BACKGROUND_GREEN,

    [(int)(LC_COLOR_CYAN)] = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY, 
    [(int)(LC_COLOR_DARK_CYAN)] = BACKGROUND_BLUE | BACKGROUND_GREEN,
};

void lccol(lccolor text, lccolor bg) {
    SetConsoleTextAttribute(hcout, (WORD)(color_bg_lookup[bg] | color_text_lookup[text]));
}


/* Files */

static HANDLE llibs_GetFileHandle(const lc filename[]) {
    return CreateFileA((char*)filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

static HANDLE llibs_GetMapHandle(HANDLE sys0_handle) {
    return CreateFileMappingA(sys0_handle, NULL, PAGE_READONLY, 0, 0, NULL);
}

static LPCVOID llibs_GetMapView(HANDLE sys1_handle) {
    return MapViewOfFile(sys1_handle, FILE_MAP_READ, 0, 0, 0);
}

static LONGLONG llibs_GetFileSize(HANDLE sys0_handle) {
    LARGE_INTEGER size;
    if (!GetFileSizeEx(sys0_handle, &size)) {
        return -1;
    }
    return size.QuadPart;
}

lerr lfopen(const lc filename[], lfile* file) {
    file->sys0_handle = llibs_GetFileHandle(filename);
    if (file->sys0_handle == INVALID_HANDLE_VALUE) {
        return 1;
    }
    file->sys1_handle = llibs_GetMapHandle(file->sys0_handle);
    if (file->sys1_handle == NULL) {
        CloseHandle(file->sys0_handle);
        return 2;
    }
    file->data_handle = llibs_GetMapView(file->sys1_handle);
    if (file->data_handle == NULL) {
        CloseHandle(file->sys1_handle);
        CloseHandle(file->sys0_handle);
        return 3;
    }
    file->size = llibs_GetFileSize(file->sys0_handle);
    if (file->size == -1) {
        UnmapViewOfFile(file->data_handle);
        CloseHandle(file->sys1_handle);
        CloseHandle(file->sys0_handle);
        return 4;
    }
    return L_OK;
}

lerr lfget(lc** pointer, lfile* file) {
    *pointer = (lc*)(file->data_handle);
    return L_OK;
}

lerr lfclose(lfile* file) {
    UnmapViewOfFile(file->data_handle);
    CloseHandle(file->sys1_handle);
    CloseHandle(file->sys0_handle);
    return L_OK;
}