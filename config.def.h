/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char* fonts[] = { "Cascadia Code:size=12", "FontAwesome:size=12", "Noto Color Emoji:size=12" };
static const char dmenufont[] = "Cascadia Code:size=12";

static const char col_cyan  []    = "#005577";
static const char col_black []    = "#000000";
static const char col_gray1 []    = "#111111";
static const char col_gray2 []    = "#444444";
static const char col_gray3 []    = "#bbbbbb";
static const char col_gray4 []    = "#777777";
static const char col_orange[]    = "#e67505";
static const char col_yellow[]    = "#ffdd00";
static const char col_white []    = "#eeeeee";

/* \x0c to \x1f in scripts to use colors */
static const char col1[]            = "#009900"; // \x0c
static const char col2[]            = "#e67505"; // \x0d
static const char col3[]            = "#990000"; // \x0e
static const char col4[]            = "#ffdd00"; // \x0f
static const char col5[]            = "#005577"; // \x10
static const char col6[]            = "#0088ff"; // \x11

enum { SchemeNorm, SchemeCol1, SchemeCol2, SchemeCol3, SchemeCol4,
       SchemeCol5, SchemeCol6, SchemeSel, SchemeStatus, SchemeTagsSel, SchemeTagsNorm, SchemeInfoSel, SchemeInfoNorm }; /* color schemes */

static const char *colors[][3]      = {
    /*                   fg            bg            border    */
    [SchemeNorm]     = { col_gray3   , col_gray1   , col_gray2 }, // \x0b
    [SchemeCol1]     = { col1        , col_gray1   , col_gray2 }, // \x0c
    [SchemeCol2]     = { col2        , col_gray1   , col_gray2 }, // \x0d
    [SchemeCol3]     = { col3        , col_gray1   , col_gray2 }, // \x0e
    [SchemeCol4]     = { col4        , col_gray1   , col_gray2 }, // \x0f
    [SchemeCol5]     = { col5        , col_gray1   , col_gray2 }, // \x10
    [SchemeCol6]     = { col6        , col_gray1   , col_gray2 }, // \x11
    [SchemeSel]      = { col_gray1   , col_yellow  , col_gray4 },
    [SchemeStatus]   = { col_gray3   , col_gray1   , col_black },
    [SchemeTagsSel]  = { col_yellow  , col_gray1   , col_black },
    [SchemeTagsNorm] = { col_gray3   , col_gray1   , col_black },
    [SchemeInfoSel]  = { col_white   , col_gray1   , col_black },
    [SchemeInfoNorm] = { col_gray2   , col_gray1   , col_black },
};
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};    /* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* tagging */
// static const char *tags[] = { "web", "a", "b", "c", "comm" };
static const char *tags[] = { "", "", "", "", "", "a", "b", "c" };

static const Rule rules[] = {
    /* class                instance     title                 tags mask  iscentered isfloating  isterminal  noswallow  monitor */
    { "Alacritty",          NULL,        NULL,                 0,         0,         0,          1,           0,        -1 },
    { "Alacritty",          NULL,        "scratchpad",         0,         1,         1,          1,           0,        -1 },
    { "Gimp",               NULL,        NULL,                 0,         0,         0,          0,           0,        -1 },
    { NULL,                 NULL,        "Emulator",           0,         0,         1,          0,           1,        -1 },
    { NULL,                 NULL,        "Event Tester",       0,         0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile             },
    { "[M]",      monocle          },
    { "[D]",      deck             },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]   = { "dmenu_run", "-i", "-m", dmenumon, NULL };
static const char *termcmd[]    = { "alacritty",                       NULL };
static const char *padcmd[]     = { "alacritty", "-t", "scratchpad",   NULL };

static const char *browsercmd[] = { "firefox"        , NULL     };
static const char *bwlockcmd[]  = { "rbw", "lock"    , NULL     };
static const char *clipcmd[]    = { "clipmenu"       , NULL     };
static const char *emojicmd[]   = { "dmenu_emoji"    , NULL     };
static const char *lockcmd[]    = { "slock"          , NULL     };
static const char *powmencmd[]  = { "dmenu_powermenu", NULL     };
static const char *pscmd[]      = { "dmenu_ps"       , NULL     };
static const char *scrotcmd[]   = { "dmenu_scrot"    , NULL     };
static const char *assistcmd[]  = { "dmenu_tuxi"     , NULL     };

static const char *layoutmenucmd = "dwm-layoutmenu";

static const char *audioup[]      = { "update_audio", "2%+"    };
static const char *audiodown[]    = { "update_audio", "2%-"    };
static const char *audiotoggle[]  = { "update_audio", "toggle" };
static const char *mictoggle[]    = { "toggle_mic" };

static const char *brightnessup[]   = { "update_brightness", "+100" }; 
static const char *brightnessdown[] = { "update_brightness", "100-" }; 

#include "movestack.c"
#include <X11/XF86keysym.h>
static Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_space          , spawn,          { .v = dmenucmd           } },

    { MODKEY,                       XK_Return         , spawn,          { .v = termcmd            } },
    { MODKEY|ControlMask,           XK_Return         , spawn,          { .v = padcmd             } },
    { MODKEY|ShiftMask,             XK_Return         , zoom,           { 0                       } },

    { MODKEY,                       XK_q              , killclient,     { 0                       } },
    { MODKEY|ShiftMask,             XK_q              , quit,           { 0                       } },

    { MODKEY,                       XK_d              , togglebar,      { 0                       } },

    { MODKEY,                       XK_h              , setmfact,       { .f = -0.05              } },
    { MODKEY,                       XK_l              , setmfact,       { .f = +0.05              } },
    { MODKEY|ShiftMask,             XK_h              , setcfact,       { .f = +0.25              } },
    { MODKEY|ShiftMask,             XK_l              , setcfact,       { .f = -0.25              } },
    { MODKEY|ShiftMask,             XK_o              , setcfact,       { .f =  0.00              } },
    
    { MODKEY,                       XK_j              , focusstack,     { .i = +1                 } },
    { MODKEY,                       XK_k              , focusstack,     { .i = -1                 } },
    { MODKEY|ShiftMask,             XK_j              , movestack,      { .i = +1                 } },
    { MODKEY|ShiftMask,             XK_k              , movestack,      { .i = -1                 } },

    { MODKEY,                       XK_comma          , focusmon,       { .i = -1                 } },
    { MODKEY,                       XK_period         , focusmon,       { .i = +1                 } },
    { MODKEY|ShiftMask,             XK_comma          , tagmon,         { .i = -1                 } },
    { MODKEY|ShiftMask,             XK_period         , tagmon,         { .i = +1                 } },
    { MODKEY|ShiftMask|ControlMask, XK_comma          , tagswapmon,     { .i = +1                 } },
    { MODKEY|ShiftMask|ControlMask, XK_period         , tagswapmon,     { .i = -1                 } },

    { MODKEY,                       XK_equal          , incnmaster,     { .i = +1                 } },
    { MODKEY,                       XK_minus          , incnmaster,     { .i = -1                 } },

    { MODKEY,                       XK_Tab            , view,           { 0                       } },
    { MODKEY,                       XK_m              , setlayout,      { 0                       } },
    { MODKEY,                       XK_t              , setlayout,      { .v = &layouts[0]        } },
    { MODKEY|ShiftMask,             XK_t              , setlayout,      { .v = &layouts[1]        } },
    { MODKEY|ControlMask,           XK_t              , setlayout,      { .v = &layouts[2]        } },
    { MODKEY,                       XK_f              , togglefloating, { 0                       } },
    { MODKEY|ShiftMask,             XK_s              , togglesticky,   { 0                       } },

    { MODKEY          ,             XK_b              , spawn,          { .v = browsercmd         } },
    { MODKEY          ,             XK_u              , spawn,     SHCMD( "sleep 0.2s; dmenu_rbw" ) }, // SHCMD to make the popup work
    { MODKEY|ShiftMask,             XK_u              , spawn,          { .v = bwlockcmd          } },
    { MODKEY          ,             XK_c              , spawn,          { .v = clipcmd            } },
    { MODKEY          ,             XK_e              , spawn,          { .v = emojicmd           } },
    { MODKEY|ShiftMask,             XK_p              , spawn,          { .v = powmencmd          } },
    { MODKEY          ,             XK_p              , spawn,          { .v = pscmd              } },
    { MODKEY          ,             XK_s              , spawn,          { .v = scrotcmd           } },
    { MODKEY|ShiftMask,             XK_space          , spawn,          { .v = assistcmd          } },
    { MODKEY          ,             XK_Escape         , spawn,          { .v = lockcmd            } },
    { MODKEY|ShiftMask,             XK_Escape         , spawn,     SHCMD( "systemctl suspend; slock"     ) },

    { 0, XF86XK_AudioLowerVolume, spawn, { .v = audiodown   } },
    { 0, XF86XK_AudioRaiseVolume, spawn, { .v = audioup     } },
    { 0, XF86XK_AudioMute       , spawn, { .v = audiotoggle } },
    { 0, XF86XK_AudioMicMute    , spawn, { .v = mictoggle   } },

    { 0, XF86XK_MonBrightnessUp  , spawn, { .v = brightnessup   } },
    { 0, XF86XK_MonBrightnessDown, spawn, { .v = brightnessdown } },

    { MODKEY,                       XK_parenright     , view,           { .ui = ~0         } },
    { MODKEY|ShiftMask,             XK_parenright     , tag,            { .ui = ~0         } },

    TAGKEYS ( XK_exclam      , 0 )
    TAGKEYS ( XK_at          , 1 )
    TAGKEYS ( XK_numbersign  , 2 )
    TAGKEYS ( XK_dollar      , 3 )
    TAGKEYS ( XK_percent     , 4 )
    TAGKEYS ( XK_asciicircum , 5 )
    TAGKEYS ( XK_ampersand   , 6 )
    TAGKEYS ( XK_asterisk    , 7 )
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      { 0      } },
    { ClkLtSymbol,          0,              Button3,        layoutmenu,     { 0      } },
    { ClkWinTitle,          0,              Button2,        killclient,     { 0      } },
    { ClkStatusText,        0,              Button1,        sigdwmblocks,   { .i = 1 } },
    { ClkStatusText,        0,              Button2,        sigdwmblocks,   { .i = 2 } },
    { ClkStatusText,        0,              Button3,        sigdwmblocks,   { .i = 3 } },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      { 0      } },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, { 0      } },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    { 0      } },
    { ClkTagBar,            0,              Button1,        view,           { 0      } },
    { ClkTagBar,            0,              Button3,        toggleview,     { 0      } },
    { ClkTagBar,            MODKEY,         Button1,        tag,            { 0      } },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      { 0      } },
};

