#include <windows.h>
#include <filter.h>

FILTER_DLL filter = {
    FILTER_FLAG_ALWAYS_ACTIVE | FILTER_FLAG_NO_CONFIG,
    NULL,NULL,
    const_cast<char*>("ÉåÉCÉÑÅ[ÉzÉCÅ[Éã"),
    NULL,NULL,NULL,
    NULL,NULL,
    NULL,NULL,NULL,
    NULL,
    func_init,
};
EXTERN_C FILTER_DLL __declspec(dllexport)* __stdcall GetFilterTable(void) {
    return &filter;
}


static int exedit_base;
static HWND exedit_hwnd;

FILTER* get_exeditfp(FILTER* fp) {
    SYS_INFO si;
    fp->exfunc->get_sys_info(NULL, &si);

    for (int i = 0; i < si.filter_n; i++) {
        FILTER* tfp = (FILTER*)fp->exfunc->get_filterp(i);
        if (tfp->information != NULL) {
            if (!strcmp(tfp->information, "ägí£ï“èW(exedit) version 0.92 by ÇjÇdÇmÇ≠ÇÒ")) return tfp;
        }
    }
    return NULL;
}


BOOL exedit_ReplaceCall(DWORD exedit_address, void* new_address) {
    DWORD oldProtect;
    DWORD* address = (DWORD*)(exedit_address + exedit_base);
    if (!VirtualProtect(address, 4, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        return FALSE;
    }
    *address = (DWORD)new_address - (DWORD)address - 4;
    return VirtualProtect(address, 4, oldProtect, &oldProtect);
}


void __cdecl scroll_horizontal_wrap(int x_ofs) {
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(exedit_hwnd, &point);
    if (point.x < 64) { // vertical
        int timeline_frame_pos = *(int*)(exedit_base + 0x1a52f0);
        int timeline_layer_pos = *(int*)(exedit_base + 0x1a5308);
        int wheel = (timeline_frame_pos < x_ofs) * 2 + -1;
        reinterpret_cast<void(__cdecl*)(int, BOOL)>(exedit_base + 0x38b70)(timeline_layer_pos + wheel, TRUE);
    } else { // horizontal
        reinterpret_cast<void(__cdecl*)(int)>(exedit_base + 0x38c70)(x_ofs);
    }
}

BOOL func_init(FILTER* fp) {
    FILTER* exeditfp = get_exeditfp(fp);
    if (exeditfp == NULL) {
        MessageBoxA(fp->hwnd, "ägí£ï“èW0.92Ç™å©Ç¬Ç©ÇËÇ‹ÇπÇÒÇ≈ÇµÇΩ", fp->name, MB_OK);
        return TRUE;
    }
    exedit_base = (int)exeditfp->dll_hinst;
    exedit_hwnd = exeditfp->hwnd;

    exedit_ReplaceCall(0x3df3a, &scroll_horizontal_wrap);


    return TRUE;
}
