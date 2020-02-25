#!/usr/bin/env python3

# adapted from the VCV Rack helper from Andrew Belt:
# https://github.com/VCVRack/Rack/blob/v1/helper.py

import sys
import os
import re
import json
import xml.etree.ElementTree

# Version check
f"Python 3.6+ is required"


class UserException(Exception):
    pass


def find(f, array):
	for a in array:
		if f(a):
			return f

def input_default(prompt, default=""):
	str = input(f"{prompt} [{default}]: ")
	if str == "":
		return default
	return str


def is_valid_slug(slug):
	return re.match(r'^[a-zA-Z0-9_\-]+$', slug) != None

def pascalcase(string):
	# convert a string to PascalCase for C# defs
	# adpated from stringcase by okunishinishi:
	# https://github.com/okunishinishi/python-stringcase
	string = re.sub(r"^[\-_\.]", '', str(string))
	if not string:
	    return string
	camelcase = str(string[0]).lower() + re.sub(r"[\-_\.\s]([a-z])", lambda matched: str(matched.group(1)).upper(), string[1:])

	return str(camelcase[0]).upper() + camelcase[1:]


def str_to_identifier(s):
	if not s:
		return "_"
	# Identifiers can't start with a number
	if s[0].isdigit():
		s = "_" + s
	# Capitalize first letter
	s = s[0].upper() + s[1:]
	# Replace special characters with underscore
	s = re.sub(r'\W', '_', s)
	return s


def create_module(slug, panel_filename=None, source_filename=None):
	# Check slug
	if not is_valid_slug(slug):
		raise UserException("Slug must only contain ASCII letters, numbers, '-', and '_'.")

	# Read manifest
	manifest_filename = 'plugin.json'
	with open(manifest_filename, "r") as f:
		manifest = json.load(f)

	# Check if module manifest exists
	module_manifest = find(lambda m: m['slug'] == slug, manifest['modules'])
	if module_manifest:
		print(f"Module {slug} already exists in plugin.json. Edit this file to modify the module manifest.")

	else:
		# Add module to manifest
		module_manifest = {}
		module_manifest['slug'] = slug
		module_manifest['name'] = input_default("Module name", slug)
		module_manifest['description'] = input_default("One-line description (optional)")
		tags = input_default("Tags (comma-separated, case-insensitive, see https://github.com/VCVRack/Rack/blob/v1/src/tag.cpp for list)")
		tags = tags.split(",")
		tags = [tag.strip() for tag in tags]
		if len(tags) == 1 and tags[0] == "":
			tags = []
		module_manifest['tags'] = tags

		manifest['modules'].append(module_manifest)

		# Write manifest
		with open(manifest_filename, "w") as f:
			json.dump(manifest, f, indent="  ")

		print(f"Added {slug} to {manifest_filename}")

	# Check filenames
	if panel_filename and source_filename:
		if not os.path.exists(panel_filename):
			raise UserException(f"Panel not found at {panel_filename}.")

		print(f"Panel found at {panel_filename}. Generating source file.")

		if os.path.exists(source_filename):
			if input_default(f"{source_filename} already exists. Overwrite?", "n").lower() != "y":
				return

		# Read SVG XML
		tree = xml.etree.ElementTree.parse(panel_filename)

		components = panel_to_components(tree)
		print(f"Components extracted from {panel_filename}")

		# Write source
		source = components_to_source(components, slug)

		with open(source_filename, "w") as f:
			f.write(source)
		print(f"Source file generated at {source_filename}")

		# Append model to plugin.hpp
		identifier = str_to_identifier(slug)

		# Tell user to add model to plugin.hpp and plugin.cpp
		print(f"""
To enable the module, add
extern Model* model{identifier};
to plugin.hpp, and add
p->addModel(model{identifier});
to the init() function in plugin.cpp.""")


def panel_to_components(tree):
	ns = {
		"svg": "http://www.w3.org/2000/svg",
		"inkscape": "http://www.inkscape.org/namespaces/inkscape",
	}

	# Get components layer
	root = tree.getroot()
	groups = root.findall(".//svg:g[@inkscape:label='components']", ns)
	# Illustrator uses `id` for the group name.
	if len(groups) < 1:
		groups = root.findall(".//svg:g[@id='components']", ns)
	if len(groups) < 1:
		raise UserException("Could not find \"components\" layer on panel")

	# Get circles and rects
	components_group = groups[0]
	circles = components_group.findall(".//svg:circle", ns)
	rects = components_group.findall(".//svg:rect", ns)

	components = {}
	components['params'] = []
	components['attens'] = []
	components['inputs'] = []
	components['outputs'] = []
	components['lights'] = []
	components['widgets'] = []

	for el in circles + rects:
		c = {}
		# Get name
		name = el.get('{http://www.inkscape.org/namespaces/inkscape}label')
		if name is None:
			name = el.get('id')
		name = str_to_identifier(name)
		c['name'] = name.upper()

		# Osc address is PascalCased name
		c['address'] = pascalcase(name)

		style = el.get('style')

		# Get stroke for uni/bi directional update
		stroke_defined = re.findall(r'stroke:none;', style)
		c['bi'] = len(stroke_defined) == 0

		# Get color
		color_match = re.search(r'fill:\S*#(.{6});', style)
		color = color_match.group(1).lower()
		c['color'] = color

		# Get position
		if el.tag == "{http://www.w3.org/2000/svg}rect":
			x = float(el.get('x'))
			y = float(el.get('y'))
			width = float(el.get('width'))
			height = float(el.get('height'))
			c['x'] = abs(round(x, 3))
			c['y'] = abs(round(y, 3))
			c['width'] = abs(round(width, 3))
			c['height'] = abs(round(height, 3))
			c['cx'] = abs(round(x + width / 2, 3))
			c['cy'] = abs(round(y + height / 2, 3))
		elif el.tag == "{http://www.w3.org/2000/svg}circle":
			cx = float(el.get('cx'))
			cy = float(el.get('cy'))
			c['cx'] = abs(round(cx, 3))
			c['cy'] = abs(round(cy, 3))

		if color == 'ff0000':
			c['size'] = "regular"
			components['params'].append(c)
			if "_atten" in name.lower():
			    components['attens'].append(c)
		if color == '00ffff':
			c['size'] = "small"
			components['params'].append(c)
			if "_atten" in name.lower():
			    components['attens'].append(c)
		if color == '00ff00':
			components['inputs'].append(c)
		if color == '0000ff':
			components['outputs'].append(c)
		if color == 'ff00ff':
			components['lights'].append(c)
		if color == 'ffff00':
			components['widgets'].append(c)

	# Sort components
	top_left_sort = lambda w: (w['cy'], w['cx'])
	components['params'] = sorted(components['params'], key=top_left_sort)
	components['attens'] = sorted(components['attens'], key=top_left_sort)
	components['inputs'] = sorted(components['inputs'], key=top_left_sort)
	components['outputs'] = sorted(components['outputs'], key=top_left_sort)
	components['lights'] = sorted(components['lights'], key=top_left_sort)
	components['widgets'] = sorted(components['widgets'], key=top_left_sort)

	print(f"Found {len(components['params'])} params, {len(components['inputs'])} inputs, {len(components['outputs'])} outputs, {len(components['lights'])} lights, and {len(components['widgets'])} custom widgets.")
	return components


def components_to_source(components, slug):
	identifier = str_to_identifier(slug)
	source = ""

	source += f"""#include "UModule.hpp"

struct {identifier} : URack::UModule {{"""

	# Params
	source += """
	enum ParamIds {"""
	for c in components['params']:
		source += f"""
		{c['name']}_PARAM,"""
	source += """
		NUM_PARAMS
	};"""

	# Inputs
	source += """
	enum InputIds {"""
	for c in components['inputs']:
		source += f"""
		{c['name']}_INPUT,"""
	source += """
		NUM_INPUTS
	};"""

	# Outputs
	source += """
	enum OutputIds {"""
	for c in components['outputs']:
		source += f"""
		{c['name']}_OUTPUT,"""
	source += """
		NUM_OUTPUTS
	};"""

	# Lights
	source += """
	enum LightIds {"""
	for c in components['lights']:
		source += f"""
		{c['name']}_LIGHT,"""
	source += """
		ACTIVE_LIGHT,
		NUM_LIGHTS
	};"""


	source += f"""

	{identifier}() {{
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);"""

	for c in components['params']:
		
		if ("_ATTEN" in c['name']):
		    continue

		has_atten = False
		for a in components['attens']:
			if c['name'] in a['name']:
			    has_atten = True
		has_input = False
		for i in components['inputs']:
			if c['name'] == i['name']:
			    has_input = True
		
		if c['name'].lower() == "active":
		    if (has_input):
			    source += f"""
		configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);"""
		    else:
			    source += f"""
		configActivate(ACTIVE_PARAM, ACTIVE_LIGHT)"""
		    continue

		if (has_atten and has_input and c['bi']):
		    source += f"""
		configBiUpdate(\"{c['address']}\", {c['name']}_PARAM, {c['name']}_INPUT, {c['name']}_ATTEN_PARAM, 0.f);"""
		elif (has_atten and has_input):
		    source += f"""
		configUpdate(\"{c['address']}\", {c['name']}_PARAM, {c['name']}_INPUT, {c['name']}_ATTEN_PARAM, 0.f);"""
		elif (has_input and c['bi'] and not has_atten):
		    source += f"""
		configBiUpdate(\"{c['address']}\", {c['name']}_PARAM, {c['name']}_INPUT);"""
		elif (has_input and not has_atten):
		    source += f"""
		configUpdate(\"{c['address']}\", {c['name']}_PARAM, {c['name']}_INPUT);"""
		elif (c['bi']):
		    source += f"""
		configBiUpdate(\"{c['address']}\", {c['name']}_PARAM);"""
		else:
		    source += f"""
		configUpdate(\"{c['address']}\", {c['name']}_PARAM);"""
	
	for c in components['outputs']:
		if "pointcloud" not in c['name'].lower() and "point_cloud" not in c['name'].lower():
			source += f"""
		configListener("{c['address']}", {c['name']}_OUTPUT);"""

	source += """
	}

	void update(const ProcessArgs& args) override {
	}
};"""

	source += f"""


struct {identifier}Widget : URack::UModuleWidget {{
	{identifier}Widget({identifier}* module) {{
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/{slug}.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,	RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));"""


	# Params
	if len(components['params']) > 0:
		source += "\n"
	for c in components['params']:
                if "ACTIVE" in c['name']:
                        source += f"""
                addParam(createParamCentered<LEDBezel>(mm2px(Vec({c['cx']}, {c['cy']})), module, {identifier}::ACTIVE_PARAM));
                addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec({c['cx']}, {c['cy']})), module, {identifier}::ACTIVE_LIGHT));"""
                        continue

                knob = "Davies1900hWhiteKnob"
                if "_ATTEN" in c['name']:
                        knob = "TrimpotGray"
                if c['size'] == "small":
                        knob = "Davies1900hSmallWhiteKnob"
                if 'x' in c:
                        source += f"""
		addParam(createParam<{knob}>(mm2px(Vec({c['x']}, {c['y']})), module, {identifier}::{c['name']}_PARAM));"""
                else:
                        source += f"""
		addParam(createParamCentered<{knob}>(mm2px(Vec({c['cx']}, {c['cy']})), module, {identifier}::{c['name']}_PARAM));"""

	# Inputs
	if len(components['inputs']) > 0:
		source += "\n"
	for c in components['inputs']:
		if "pointcloud" in c['name'].lower() or "point_cloud" in c['name'].lower():
			source += f"""
		addPointCloudInput(mm2px(Vec({c['cx']}, {c['cy']})), module, {identifier}::{c['name']}_INPUT, "{c['address']}Input");"""
		elif 'x' in c:
			source += f"""
		addInput(createInput<PJ301MPort>(mm2px(Vec({c['x']}, {c['y']})), module, {identifier}::{c['name']}_INPUT));"""
		else:
			source += f"""
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec({c['cx']}, {c['cy']})), module, {identifier}::{c['name']}_INPUT));"""

	# Outputs
	if len(components['outputs']) > 0:
		source += "\n"
	for c in components['outputs']:
		if "pointcloud" in c['name'].lower() or "point_cloud" in c['name'].lower():
			source += f"""
		addPointCloudOutput(mm2px(Vec({c['cx']}, {c['cy']})), module, {identifier}::{c['name']}_INPUT, "{c['address']}Output");"""
		elif 'x' in c:
			source += f"""
		addOutput(createOutput<PJ301MPort>(mm2px(Vec({c['x']}, {c['y']})), module, {identifier}::{c['name']}_OUTPUT));"""
		else:
			source += f"""
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec({c['cx']}, {c['cy']})), module, {identifier}::{c['name']}_OUTPUT));"""

	# Lights
	if len(components['lights']) > 0:
		source += "\n"
	for c in components['lights']:
		if 'x' in c:
			source += f"""
		addChild(createLight<MediumLight<RedLight>>(mm2px(Vec({c['x']}, {c['y']})), module, {identifier}::{c['name']}_LIGHT));"""
		else:
			source += f"""
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec({c['cx']}, {c['cy']})), module, {identifier}::{c['name']}_LIGHT));"""

	# Widgets
	if len(components['widgets']) > 0:
		source += "\n"
	for c in components['widgets']:
		if 'x' in c:
			source += f"""
		// mm2px(Vec({c['width']}, {c['height']}))
		addChild(createWidget<Widget>(mm2px(Vec({c['x']}, {c['y']}))));"""
		else:
			source += f"""
		addChild(createWidgetCentered<Widget>(mm2px(Vec({c['cx']}, {c['cy']}))));"""

	source += f"""
	}}
}};


Model* model{identifier} = createModel<{identifier}, {identifier}Widget>("{slug}");"""

	return source


def usage(script):
	text = f"""VCV Rack Plugin Helper Utility

Usage: {script} <command> ...
Commands:

createmodule <module slug> [panel file] [source file]

	Adds a new module to the plugin manifest in the current directory.
	If a panel and source file are given, generates a template source file initialized with components from a panel file.
	Example:
		{script} createmodule MyModule res/MyModule.svg src/MyModule.cpp

	See https://vcvrack.com/manual/PanelTutorial.html for creating SVG panel files.
"""
	print(text)


def parse_args(args):
	script = args.pop(0)
	if len(args) == 0:
		usage(script)
		return

	cmd = args.pop(0)
	if cmd == 'createmodule':
		create_module(*args)
	elif cmd == 'pascal':
		print(pascalcase(*args))
	else:
		print(f"Command not found: {cmd}")


if __name__ == "__main__":
	try:
		parse_args(sys.argv)
	except KeyboardInterrupt:
		pass
	except UserException as e:
		print(e)
		sys.exit(1)
