type compopt &>/dev/null && {
 complete -F _dials_autocomplete dials.analyse_background
 complete -F _dials_autocomplete dials.find_spots
 complete -F _dials_autocomplete dials.geometry_viewer
 complete -F _dials_autocomplete dials.index
 complete -F _dials_autocomplete dials.integrate
 complete -F _dials_autocomplete dials.reciprocal_lattice_viewer
 complete -F _dials_autocomplete dials.refine
 complete -F _dials_autocomplete dials.reindex
 complete -F _dials_autocomplete dials.rl_png
}
type compopt &>/dev/null || {
 complete -o nospace -F _dials_autocomplete dials.analyse_background
 complete -o nospace -F _dials_autocomplete dials.find_spots
 complete -o nospace -F _dials_autocomplete dials.geometry_viewer
 complete -o nospace -F _dials_autocomplete dials.index
 complete -o nospace -F _dials_autocomplete dials.integrate
 complete -o nospace -F _dials_autocomplete dials.reciprocal_lattice_viewer
 complete -o nospace -F _dials_autocomplete dials.refine
 complete -o nospace -F _dials_autocomplete dials.reindex
 complete -o nospace -F _dials_autocomplete dials.rl_png
}
