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
static const char col_orange[]    = "#e67505";
static const char col_yellow[]    = "#ffdd00";

/* \x0b to \x1f in scripts to use colors */
static const char col1[]            = "#e67505";
static const char col2[]            = "#ffdd00";
static const char col3[]            = "#f67606";
static const char col4[]            = "#ffcc33";
static const char col5[]            = "#005577";
static const char col6[]            = "#0088ff";

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
    [SchemeSel]      = { col_gray1   , col_yellow  , col_gray2 },
    [SchemeStatus]   = { col_gray3   , col_gray1   , col_black },
    [SchemeTagsSel]  = { col_yellow  , col_gray1   , col_black },
    [SchemeTagsNorm] = { col_gray3   , col_gray1   , col_black },
    [SchemeInfoSel]  = { col_orange  , col_gray1   , col_black },
    [SchemeInfoNorm] = { col_gray2   , col_gray1   , col_black },
};
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};    /* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* tagging */
static const char *tags[] = { "web", "a", "b", "c", "comm" };

static const Rule rules[] = {
    /* class        instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
    { "Gimp",       NULL,     NULL,           0,         0,          0,           0,        -1 },
    { "Alacritty",  NULL,     NULL,           0,         0,          1,           0,        -1 },
    { NULL,         NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "[M]",      monocle },
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
static const char *dmenucmd[]   = { "dmenu_run", "-m", dmenumon };
static const char *termcmd[]    = { "alacritty"      , NULL     };

static const char *browsercmd[] = { "firefox"        , NULL     };
static const char *bwcmd[]      = { "dmenu_rbw"      , NULL     };
static const char *clipcmd[]    = { "clipmenu"       , NULL     };
static const char *emojicmd[]   = { "dmenu_emoji"    , NULL     };
static const char *lockcmd[]    = { "slock"          , NULL     };
static const char *powmencmd[]  = { "dmenu_powermenu", NULL     };
static const char *pscmd[]      = { "dmenu_ps"       , NULL     };
static const char *scrotcmd[]   = { "dmenu_scrot"    , NULL     };
static const char *assistcmd[]  = { "dmenu_tuxi"     , NULL     };

#include "movestack.c"
#include <X11/XF86keysym.h>
static Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_space          , spawn,          { .v = dmenucmd    } },

    { MODKEY,                       XK_Return         , spawn,          { .v = termcmd     } },
    { MODKEY|ShiftMask,             XK_Return         , zoom,           { 0                } },

    { MODKEY,                       XK_q              , killclient,     { 0                } },
    { MODKEY|ShiftMask,             XK_q              , quit,           { 0                } },

    { MODKEY,                       XK_d              , togglebar,      { 0                } },

    { MODKEY,                       XK_h              , setmfact,       { .f = -0.05       } },
    { MODKEY,                       XK_l              , setmfact,       { .f = +0.05       } },
    { MODKEY|ShiftMask,             XK_h              , setcfact,       { .f = +0.25       } },
    { MODKEY|ShiftMask,             XK_l              , setcfact,       { .f = -0.25       } },
    { MODKEY|ShiftMask,             XK_o              , setcfact,       { .f =  0.00       } },
    
    { MODKEY,                       XK_j              , focusstack,     { .i = +1          } },
    { MODKEY,                       XK_k              , focusstack,     { .i = -1          } },
    { MODKEY|ShiftMask,             XK_j              , movestack,      { .i = +1          } },
    { MODKEY|ShiftMask,             XK_k              , movestack,      { .i = -1          } },

    { MODKEY,                       XK_comma          , focusmon,       { .i = -1          } },
    { MODKEY,                       XK_period         , focusmon,       { .i = +1          } },
    { MODKEY|ShiftMask,             XK_comma          , tagmon,         { .i = -1          } },
    { MODKEY|ShiftMask,             XK_period         , tagmon,         { .i = +1          } },
    { MODKEY|ShiftMask|ControlMask, XK_comma          , tagswapmon,     { .i = +1          } },
    { MODKEY|ShiftMask|ControlMask, XK_period         , tagswapmon,     { .i = -1          } },

    { MODKEY,                       XK_equal          , incnmaster,     { .i = +1          } },
    { MODKEY,                       XK_minus          , incnmaster,     { .i = -1          } },

    { MODKEY,                       XK_Tab            , view,           { 0                } },
    { MODKEY,                       XK_m              , setlayout,      { 0                } },
    { MODKEY,                       XK_t              , setlayout,      { .v = &layouts[0] } },
    { MODKEY|ShiftMask,             XK_t              , setlayout,      { .v = &layouts[1] } },
    { MODKEY,                       XK_f              , togglefloating, { 0                } },
    { MODKEY|ShiftMask,             XK_s              , togglesticky,   { 0                } },

    { MODKEY          ,             XK_b              , spawn,          { .v = browsercmd  } },
    { MODKEY          ,             XK_u              , spawn,          { .v = bwcmd       } },
    { MODKEY          ,             XK_c              , spawn,          { .v = clipcmd     } },
    { MODKEY          ,             XK_e              , spawn,          { .v = emojicmd    } },
    { MODKEY|ShiftMask,             XK_p              , spawn,          { .v = powmencmd   } },
    { MODKEY          ,             XK_p              , spawn,          { .v = pscmd       } },
    { MODKEY          ,             XK_s              , spawn,          { .v = scrotcmd    } },
    { MODKEY          ,             XK_x              , spawn,          { .v = assistcmd   } },
    { MODKEY          ,             XK_Escape         , spawn,          { .v = lockcmd     } },

    { 0, XF86XK_AudioLowerVolume, spawn, SHCMD("amixer -q sset Master 2%-; sigdwmblocks 9")   },
    { 0, XF86XK_AudioRaiseVolume, spawn, SHCMD("amixer -q sset Master 2%+; sigdwmblocks 9")   },
    { 0, XF86XK_AudioMute       , spawn, SHCMD("amixer set Master toggle; sigdwmblocks 9") },

    { 0, XF86XK_MonBrightnessUp  , spawn, SHCMD("brightnessctl s +100") },
    { 0, XF86XK_MonBrightnessDown, spawn, SHCMD("brightnessctl s 100-") },

    { MODKEY,                       XK_parenright     , view,           { .ui = ~0         } },
    { MODKEY|ShiftMask,             XK_parenright     , tag,            { .ui = ~0         } },

    TAGKEYS ( XK_exclam      , 0 )
    TAGKEYS ( XK_at          , 1 )
    TAGKEYS ( XK_numbersign  , 2 )
    TAGKEYS ( XK_dollar      , 3 )
    TAGKEYS ( XK_percent     , 4 )
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
    { ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
    { ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

