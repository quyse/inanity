add_definitions(-DHAVE_OT)

add_library(inanity-dep-harfbuzz
	# base
	repo/src/hb-blob.cc
	repo/src/hb-buffer-serialize.cc
	repo/src/hb-buffer.cc
	repo/src/hb-common.cc
	repo/src/hb-fallback-shape.cc
	repo/src/hb-face.cc
	repo/src/hb-font.cc
	repo/src/hb-ot-tag.cc
	repo/src/hb-set.cc
	repo/src/hb-shape.cc
	repo/src/hb-shape-plan.cc
	repo/src/hb-shaper.cc
	repo/src/hb-tt-font.cc
	repo/src/hb-unicode.cc
	repo/src/hb-warning.cc

	# opentype
	repo/src/hb-ot-layout.cc
	repo/src/hb-ot-map.cc
	repo/src/hb-ot-shape.cc
	repo/src/hb-ot-shape-complex-arabic.cc
	repo/src/hb-ot-shape-complex-default.cc
	repo/src/hb-ot-shape-complex-indic.cc
	repo/src/hb-ot-shape-complex-indic-table.cc
	repo/src/hb-ot-shape-complex-myanmar.cc
	repo/src/hb-ot-shape-complex-sea.cc
	repo/src/hb-ot-shape-complex-thai.cc
	repo/src/hb-ot-shape-normalize.cc
	repo/src/hb-ot-shape-fallback.cc

	# freetype
	repo/src/hb-ft.cc
)
