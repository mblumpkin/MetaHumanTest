"""
make_ui_textures.py
===================

Builds every PNG texture the IHPC (Avatar Injury Prevention) UI needs, using the
assets exported from the Figma file "Avatar Injury Prevention Program".

Run from the project root:
    python -I Tools/FigmaUI/make_ui_textures.py

Input : SourceArt/Figma/Original/   (raw files downloaded from Figma)
Output: SourceArt/Figma/UE/         (PNG files ready to import into Unreal)

Why a script?
  * Unreal cannot import SVG files, so the small vector icons from Figma
    (toggle, chevrons, zoom) are rasterised here from their exact SVG numbers.
  * The video-card art in Figma has the text baked into the picture. The UI needs
    the text to come from data, so this script rebuilds a text-free card
    background from the original pixels.
  * Anyone on the team can re-run it after the Figma file changes.

Every icon is rendered at 4x its Figma size so it stays sharp on high-DPI screens.
"""

import math
import os

import numpy as np
from PIL import Image, ImageDraw

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SRC = os.path.join(ROOT, "SourceArt", "Figma", "Original")
OUT = os.path.join(ROOT, "SourceArt", "Figma", "UE")

ICON_SCALE = 4      # final pixels per Figma pixel
SUPERSAMPLE = 8     # draw this much bigger, then shrink, for smooth (anti-aliased) edges


# ---------------------------------------------------------------------------
# Small drawing helpers
# ---------------------------------------------------------------------------

def new_canvas(w, h):
    """Transparent canvas big enough to draw a w x h (Figma px) icon supersampled."""
    k = ICON_SCALE * SUPERSAMPLE
    return Image.new("RGBA", (round(w * k), round(h * k)), (0, 0, 0, 0)), k


def finish(img, w, h, name):
    """Shrink the supersampled canvas to 4x size and save it."""
    final = img.resize((round(w * ICON_SCALE), round(h * ICON_SCALE)), Image.LANCZOS)
    path = os.path.join(OUT, name)
    final.save(path)
    print("wrote", os.path.relpath(path, ROOT), final.size)


def rgba(hex_color, opacity=1.0):
    """'#RRGGBB' + opacity (0..1) -> (r, g, b, a) tuple."""
    hex_color = hex_color.lstrip("#")
    r, g, b = (int(hex_color[i:i + 2], 16) for i in (0, 2, 4))
    return (r, g, b, round(255 * opacity))


def layer(base, draw_fn):
    """Draw on a separate transparent layer, then alpha-composite it onto base.
    This reproduces how Figma stacks semi-transparent shapes."""
    top = Image.new("RGBA", base.size, (0, 0, 0, 0))
    draw_fn(ImageDraw.Draw(top))
    return Image.alpha_composite(base, top)


def stroke_polyline(draw, pts, k, width, color):
    """Polyline with round caps and round joins (SVG stroke-linecap/linejoin=round)."""
    scaled = [(x * k, y * k) for x, y in pts]
    w = width * k
    draw.line(scaled, fill=color, width=round(w))
    r = w / 2
    for x, y in scaled:  # round caps + joins
        draw.ellipse([x - r, y - r, x + r, y + r], fill=color)


# ---------------------------------------------------------------------------
# 1. Toggle switch (Figma "Component 4", 38 x 20)
# ---------------------------------------------------------------------------

def make_toggle(name, track_hex, track_opacity, knob_cx, knob_opacity):
    img, k = new_canvas(38, 20)
    # <rect width="38" height="20" rx="10" fill=track>
    img = layer(img, lambda d: d.rounded_rectangle([0, 0, 38 * k, 20 * k], radius=10 * k,
                                                   fill=rgba(track_hex, track_opacity)))
    # <circle cx=knob_cx cy="10" r="8.5" fill="white">
    img = layer(img, lambda d: d.ellipse([(knob_cx - 8.5) * k, 1.5 * k, (knob_cx + 8.5) * k, 18.5 * k],
                                         fill=rgba("#FFFFFF", knob_opacity)))
    finish(img, 38, 20, name)


# ---------------------------------------------------------------------------
# 2. Chevrons
# ---------------------------------------------------------------------------

def make_small_chevron():
    """Accordion chevron (Figma vector 15 x 9.52635, stroke 2, white).
    The SVG path points UP; the DOWN version is a vertical flip."""
    w, h = 15, 9.52635
    img, k = new_canvas(w, h)
    d = ImageDraw.Draw(img)
    stroke_polyline(d, [(1.00003, 8.52632), (7.15793, 1), (14, 8.52632)], k, 2, rgba("#FFFFFF"))
    finish(img, w, h, "T_IHPC_Chevron_Up.png")
    finish(img.transpose(Image.FLIP_TOP_BOTTOM), w, h, "T_IHPC_Chevron_Down.png")


def make_header_chevron(name_up, name_down, hex_color):
    """Header toggle chevron (Figma 'Vector 2', 27 x 12, stroke 5)."""
    w, h = 27.0006, 12.0003
    img, k = new_canvas(w, h)
    d = ImageDraw.Draw(img)
    stroke_polyline(d, [(2.50032, 9.5), (13.5003, 2.5), (24.5003, 9.5)], k, 5, rgba(hex_color))
    finish(img, w, h, name_up)
    if name_down:
        finish(img.transpose(Image.FLIP_TOP_BOTTOM), w, h, name_down)


# ---------------------------------------------------------------------------
# 3. Zoom icon (Figma 'Vector', 20 x 19, stroke 2, white)
# ---------------------------------------------------------------------------

def make_zoom():
    w, h = 20, 19
    img, k = new_canvas(w, h)
    d = ImageDraw.Draw(img)
    white = rgba("#FFFFFF")
    # magnifier ring: centre (8.92, 9.107), radii 7.92 x 8.107
    cx, cy, rx, ry = 8.92, 9.107, 7.92, 8.107
    ring = [(cx + rx * math.cos(t), cy + ry * math.sin(t))
            for t in np.linspace(0, 2 * math.pi, 181)]
    stroke_polyline(d, ring, k, 2, white)
    stroke_polyline(d, [(5.32, 9.10699), (12.52, 9.10699)], k, 2, white)   # plus: horizontal
    stroke_polyline(d, [(8.92, 5.422), (8.92, 12.792)], k, 2, white)       # plus: vertical
    stroke_polyline(d, [(15.0789, 14.4069), (19, 18)], k, 2, white)        # handle
    finish(img, w, h, "T_IHPC_Zoom.png")


# ---------------------------------------------------------------------------
# 4. Raster icons supplied as PNG in Figma (just resized)
# ---------------------------------------------------------------------------

def resize_png(src_name, figma_w, figma_h, out_name):
    im = Image.open(os.path.join(SRC, src_name)).convert("RGBA")
    im = im.resize((figma_w * ICON_SCALE, figma_h * ICON_SCALE), Image.LANCZOS)
    path = os.path.join(OUT, out_name)
    im.save(path)
    print("wrote", os.path.relpath(path, ROOT), im.size)


# ---------------------------------------------------------------------------
# 5. Gradients that UMG cannot draw by itself
# ---------------------------------------------------------------------------

def make_title_glow():
    """Glow behind the title text: Figma linear-gradient, colour rgb(211,216,169),
    alpha 0 at 12.5%, 0.35 at 51.923%, 0 at 89.904% (576 x 54 in Figma)."""
    w, h = 512, 8
    stops = [(0.0, 0.0), (0.125, 0.0), (0.51923, 0.35), (0.89904, 0.0), (1.0, 0.0)]
    arr = np.zeros((h, w, 4), dtype=np.uint8)
    arr[..., 0], arr[..., 1], arr[..., 2] = 211, 216, 169
    xs = np.linspace(0, 1, w)
    alpha = np.interp(xs, [s[0] for s in stops], [s[1] for s in stops])
    arr[..., 3] = (alpha * 255).round().astype(np.uint8)[None, :]
    path = os.path.join(OUT, "T_IHPC_TitleGlow.png")
    Image.fromarray(arr).save(path)
    print("wrote", os.path.relpath(path, ROOT), (w, h))


def make_drop_shadow():
    """Shadow under the title bar / nav bar: CSS box-shadow 0 7px 6.3px rgba(0,0,0,0.25).
    One column of pixels: row d = d pixels below the bar's bottom edge."""
    rows = 24
    sigma = 6.3 / 2.0  # CSS blur radius = 2 x standard deviation
    arr = np.zeros((rows, 4, 4), dtype=np.uint8)
    for d in range(rows):
        # Fraction of the offset shadow still covering this row after the blur.
        coverage = 0.5 * math.erfc((d + 0.5 - 7.0) / (sigma * math.sqrt(2)))
        arr[d, :, 3] = round(255 * 0.25 * coverage)
    path = os.path.join(OUT, "T_IHPC_DropShadow.png")
    Image.fromarray(arr).save(path)
    print("wrote", os.path.relpath(path, ROOT), (4, rows))


# ---------------------------------------------------------------------------
# 6. Text-free video card background
# ---------------------------------------------------------------------------

def make_video_card_background():
    """The Figma 'videos row' uses one picture (video_cards.png) that holds three
    finished cards, text included. Rebuild one card with no text and no white
    severity tick, so Unreal can draw the title/date/time/tick from data.

    Measured from the picture (pixels in video_cards.png):
      card 1 x 49-724, card 2 x 1029-1704, card 3 x 2009-2684, all y 44-311
      white tick: 8 px wide, rows 45-82 (card 1 x_rel 114-121, card 3 x_rel 592-599)
      severity bar: rows 57-70, body: rows 71-311
      text starts at x_rel ~237, so x_rel >= 232 is plain left-to-right gradient
    """
    src = np.array(Image.open(os.path.join(SRC, "video_cards.png")).convert("RGBA")).astype(np.int32)
    top, bottom = 44, 311
    card1 = src[top:bottom + 1, 49:725].copy()
    card3 = src[top:bottom + 1, 2009:2685].copy()

    out = card3.copy()                                      # icon panel + bar from card 3
    out[:, 586:606] = card1[:, 586:606]                    # remove card 3's tick using card 1 pixels

    # Remove the text: every text-free row of the body is identical, so copy one
    # clean row (y=280 in the source) down the whole body, keeping the alpha
    # (which holds the rounded bottom corners).
    clean_row = 280 - top
    body_top = 71 - top
    for x in range(232, out.shape[1]):
        col = clean_row
        # near the rounded right corner, borrow the colour from the nearest solid column
        sx = x
        while out[col, sx, 3] < 255 and sx > 232:
            sx -= 1
        out[body_top:, x, :3] = out[col, sx, :3]

    img = Image.fromarray(out.clip(0, 255).astype(np.uint8))
    path = os.path.join(OUT, "T_IHPC_VideoCard_BG.png")
    img.save(path)
    print("wrote", os.path.relpath(path, ROOT), img.size)


def main():
    os.makedirs(OUT, exist_ok=True)
    make_toggle("T_IHPC_Toggle_Off.png", "#D9D9D9", 0.20, 10.5, 0.65)
    make_toggle("T_IHPC_Toggle_On.png", "#B3CA7F", 0.76, 27.5, 0.75)
    make_small_chevron()
    make_header_chevron("T_IHPC_HeaderChevron_Up.png", "T_IHPC_HeaderChevron_Down.png", "#FFFFFF")
    make_header_chevron("T_IHPC_HeaderChevron_Up_Blue.png", None, "#245983")
    make_zoom()
    resize_png("download_icon.png", 30, 30, "T_IHPC_Download.png")
    resize_png("inspect_code.png", 33, 33, "T_IHPC_InspectCode.png")
    make_title_glow()
    make_drop_shadow()
    make_video_card_background()


if __name__ == "__main__":
    main()
