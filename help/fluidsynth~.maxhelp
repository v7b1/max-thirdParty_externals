{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 7,
			"minor" : 0,
			"revision" : 5,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"rect" : [ 175.0, 79.0, 787.0, 831.0 ],
		"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
		"editing_bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"boxes" : [ 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-1",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 270.0, 626.0, 463.0, 17.0 ],
					"style" : "",
					"text" : "(Note that 'tuning-keys' and 'tuning-octave' set a tuning program that has to be explicitly called with 'tuning-select')"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-2",
					"maxclass" : "incdec",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 397.0, 742.0, 16.0, 19.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"htricolor" : [ 0.87, 0.82, 0.24, 1.0 ],
					"id" : "obj-3",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 360.0, 742.0, 35.0, 19.0 ],
					"style" : "",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"tricolor" : [ 0.75, 0.75, 0.75, 1.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"blackkeycolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"hkeycolor" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"id" : "obj-4",
					"maxclass" : "kslider",
					"numinlets" : 2,
					"numoutlets" : 2,
					"offset" : 24,
					"outlettype" : [ "int", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 360.0, 704.0, 392.0, 34.0 ],
					"presentation_rect" : [ 0.0, 0.0, 392.0, 35.0 ],
					"range" : 96,
					"style" : "",
					"whitekeycolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"hidden" : 1,
					"id" : "obj-5",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 70.0, 693.0, 67.0, 19.0 ],
					"style" : "",
					"text" : "loadmess 120"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 24.0, 765.0, 45.0, 45.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-10",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 260.0, 583.0, 483.0, 17.0 ],
					"style" : "",
					"text" : "set individual detuning (regarding tempered tuning) in cent for each note in the octave: <bank> <program> [<trans> ... ]"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-11",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 161.0, 562.0, 544.0, 17.0 ],
					"style" : "",
					"text" : "set individual pitch in MIDI cent for each key (0 to 127): <bank> <program> [<pitch> ... ] (the last two given values will be extrapolated)"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"hidden" : 1,
					"id" : "obj-12",
					"maxclass" : "newobj",
					"numinlets" : 3,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "" ],
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 0,
							"revision" : 5,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"rect" : [ 161.0, 79.0, 486.0, 447.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 15.0, 15.0 ],
						"gridsnaponopen" : 1,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "",
						"boxes" : [ 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-1",
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 32.0, 45.0, 15.0, 15.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-2",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 2,
									"outlettype" : [ "int", "int" ],
									"patching_rect" : [ 262.0, 278.0, 50.0, 19.0 ],
									"style" : "",
									"text" : "stripnote"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-3",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 262.0, 301.0, 62.0, 19.0 ],
									"style" : "",
									"text" : "prepend set"
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-4",
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 262.0, 366.0, 15.0, 15.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-5",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 336.0, 301.0, 62.0, 19.0 ],
									"style" : "",
									"text" : "prepend set"
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-6",
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 336.0, 366.0, 15.0, 15.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-7",
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 238.0, 45.0, 15.0, 15.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-8",
									"maxclass" : "newobj",
									"numinlets" : 3,
									"numoutlets" : 2,
									"outlettype" : [ "float", "float" ],
									"patching_rect" : [ 55.0, 105.0, 94.0, 19.0 ],
									"style" : "",
									"text" : "makenote 120 400"
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-9",
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"patching_rect" : [ 55.0, 45.0, 15.0, 15.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-10",
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 190.0, 366.0, 15.0, 15.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-11",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 313.0, 215.0, 87.0, 19.0 ],
									"style" : "",
									"text" : "control $2 $1 $3"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-12",
									"maxclass" : "newobj",
									"numinlets" : 3,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 313.0, 191.0, 58.0, 19.0 ],
									"style" : "",
									"text" : "pack 0 0 0"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-13",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "int", "int", "int" ],
									"patching_rect" : [ 313.0, 139.0, 58.0, 19.0 ],
									"style" : "",
									"text" : "ctlin"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-14",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 228.0, 214.0, 67.0, 19.0 ],
									"style" : "",
									"text" : "prepend bend"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-15",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 228.0, 191.0, 58.0, 19.0 ],
									"style" : "",
									"text" : "pack 0 0"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-16",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "int", "int" ],
									"patching_rect" : [ 228.0, 139.0, 58.0, 19.0 ],
									"style" : "",
									"text" : "bendin"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-17",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 143.0, 214.0, 67.0, 19.0 ],
									"style" : "",
									"text" : "prepend note"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-18",
									"maxclass" : "newobj",
									"numinlets" : 3,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 143.0, 191.0, 58.0, 19.0 ],
									"style" : "",
									"text" : "pack 0 0 1"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-19",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "int", "int", "int" ],
									"patching_rect" : [ 143.0, 139.0, 58.0, 19.0 ],
									"style" : "",
									"text" : "notein"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-11", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-13", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-13", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-13", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-16", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-16", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-17", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-18", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-18", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-19", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-18", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-19", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-18", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-19", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-19", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-19", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-19", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-19", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-3", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-4", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-18", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-18", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-8", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-18", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 323.0, 682.0, 84.0, 19.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p MIDI-keyboard"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-13",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 561.0, 135.0, 19.0 ],
					"style" : "",
					"text" : "tuning-keys 0 0 3000 3050"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-14",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 562.0, 604.0, 214.0, 17.0 ],
					"style" : "",
					"text" : "reset to ordinary tempered tuning for given channel"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-15",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 120.0, 604.0, 363.0, 17.0 ],
					"style" : "",
					"text" : "select tuning bank and program for given channel: <bank> <program> <channel>"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-16",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 487.0, 603.0, 74.0, 19.0 ],
					"style" : "",
					"text" : "tuning-reset 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-17",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 603.0, 95.0, 19.0 ],
					"style" : "",
					"text" : "tuning-select 0 0 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-18",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 582.0, 234.0, 19.0 ],
					"style" : "",
					"text" : "tuning-octave 0 0 50 0 50 0 50 0 50 0 50 0 50 0"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-19",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 119.0, 532.0, 170.0, 17.0 ],
					"style" : "",
					"text" : "set resampling method (default is cubic)"
				}

			}
, 			{
				"box" : 				{
					"allowdrag" : 0,
					"fontsize" : 9.0,
					"id" : "obj-20",
					"items" : [ "resample", "nearest", ",", "resample", "linear", ",", "resample", "cubic", ",", "resample", "sinc" ],
					"labelclick" : 1,
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 24.0, 530.0, 93.0, 19.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-21",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 41.0, 83.0, 183.0, 17.0 ],
					"style" : "",
					"text" : "FluidSynth home: http://www.fluidsynth.org/"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-22",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 66.0, 67.0, 188.0, 17.0 ],
					"style" : "",
					"text" : "FluidSynth is written by Peter Hanappe et al."
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-23",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 393.0, 42.0, 263.0, 17.0 ],
					"style" : "",
					"text" : "output list of presets: <name> <soundfont> <bank> <program>"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-24",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 410.0, 63.0, 339.0, 17.0 ],
					"style" : "",
					"text" : "output list of presets of given soundfont: <name> <soundfont> <bank> <program>"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-25",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 389.0, 126.0, 358.0, 17.0 ],
					"style" : "",
					"text" : "output 'off' or current chorus parameters: <level> <speed> <depth> <type> <# of units>"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-26",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 390.0, 105.0, 338.0, 17.0 ],
					"style" : "",
					"text" : "output 'off' or current reverb parameters: <level> <room size> <damping> <width>"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-27",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 405.0, 21.0, 224.0, 17.0 ],
					"style" : "",
					"text" : "output list of soundfonts: <stack index> <name> <id>"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-28",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 395.0, 84.0, 387.0, 17.0 ],
					"style" : "",
					"text" : "output selected preset for each channel: <channel> <soundfont> <bank> <program> <preset>"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-29",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 230.0, 711.0, 50.0, 19.0 ],
					"style" : "",
					"text" : "print info"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-30",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 127.0, 682.0, 122.0, 19.0 ],
					"style" : "",
					"text" : "route bang"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-31",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 327.0, 41.0, 64.0, 19.0 ],
					"style" : "",
					"text" : "info presets"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-32",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 327.0, 62.0, 81.0, 19.0 ],
					"style" : "",
					"text" : "info presets GM"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-33",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 327.0, 125.0, 60.0, 19.0 ],
					"style" : "",
					"text" : "info chorus"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-34",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 327.0, 104.0, 61.0, 19.0 ],
					"style" : "",
					"text" : "info reverb"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-35",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 327.0, 20.0, 76.0, 19.0 ],
					"style" : "",
					"text" : "info soundfonts"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-36",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 327.0, 83.0, 66.0, 19.0 ],
					"style" : "",
					"text" : "info channels"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-37",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 510.0, 202.0, 85.0, 17.0 ],
					"style" : "",
					"text" : "print list of presets"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-38",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 439.0, 201.0, 69.0, 19.0 ],
					"style" : "",
					"text" : "print presets"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-39",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 527.0, 223.0, 176.0, 17.0 ],
					"style" : "",
					"text" : "print list of presets of the given soundfont"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-40",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 439.0, 222.0, 86.0, 19.0 ],
					"style" : "",
					"text" : "print presets GM"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-41",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 672.0, 480.0, 55.0, 19.0 ],
					"style" : "",
					"text" : "reverb off"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-42",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 708.0, 501.0, 55.0, 19.0 ],
					"style" : "",
					"text" : "chorus off"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-43",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 119.0, 501.0, 112.0, 19.0 ],
					"style" : "",
					"text" : "chorus 10 0.5 40 0 10"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-44",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 654.0, 501.0, 52.0, 19.0 ],
					"style" : "",
					"text" : "chorus on"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-45",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 233.0, 502.0, 362.0, 17.0 ],
					"style" : "",
					"text" : "set chorus parameters: level, speed, depth, type, number of units or switch chorus on/off"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-46",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 501.0, 93.0, 19.0 ],
					"style" : "",
					"text" : "chorus 2 0.3 8 0 3"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-47",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 131.0, 480.0, 105.0, 19.0 ],
					"style" : "",
					"text" : "reverb 1. 0.8 0.5 0.7"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-48",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 506.0, 307.0, 137.0, 17.0 ],
					"style" : "",
					"text" : "print current chorus parameters"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-49",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 439.0, 306.0, 65.0, 19.0 ],
					"style" : "",
					"text" : "print chorus"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-50",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 507.0, 286.0, 136.0, 17.0 ],
					"style" : "",
					"text" : "print current reverb parameters"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-51",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 439.0, 285.0, 66.0, 19.0 ],
					"style" : "",
					"text" : "print reverb"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-52",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 617.0, 480.0, 53.0, 19.0 ],
					"style" : "",
					"text" : "reverb on"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-53",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 238.0, 481.0, 323.0, 17.0 ],
					"style" : "",
					"text" : "set reverb parameters: level, room size, damping, width or switch reberb on/off"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-54",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 480.0, 105.0, 19.0 ],
					"style" : "",
					"text" : "reverb 0.9 0.2 0 0.5"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-55",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 284.0, 438.0, 222.0, 17.0 ],
					"style" : "",
					"text" : "change soundfont \"generator\" value of given channel"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"format" : 6,
					"htricolor" : [ 0.87, 0.82, 0.24, 1.0 ],
					"id" : "obj-56",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 68.0, 459.0, 48.0, 19.0 ],
					"style" : "",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"tricolor" : [ 0.75, 0.75, 0.75, 1.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-57",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 459.0, 42.0, 19.0 ],
					"style" : "",
					"text" : "gain $1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-58",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 247.0, 154.0, 50.0, 19.0 ],
					"style" : "",
					"text" : "unload all"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-59",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 127.0, 711.0, 19.0, 19.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-60",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 116.0, 134.0, 70.0, 17.0 ],
					"style" : "",
					"text" : "load soundfont"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-61",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 88.0, 133.0, 26.0, 19.0 ],
					"style" : "",
					"text" : "load"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-62",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 22.0, 99.0, 243.0, 17.0 ],
					"style" : "",
					"text" : "Max/MSP integration by Norbert Schnell for Roland Cahen"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-63",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 524.0, 265.0, 238.0, 17.0 ],
					"style" : "",
					"text" : "print list of modulator values for given channel (default 1)"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-64",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 439.0, 264.0, 83.0, 19.0 ],
					"style" : "",
					"text" : "print generators"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"format" : 6,
					"htricolor" : [ 0.87, 0.82, 0.24, 1.0 ],
					"id" : "obj-65",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 91.0, 437.0, 45.0, 19.0 ],
					"style" : "",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"tricolor" : [ 0.75, 0.75, 0.75, 1.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-66",
					"maxclass" : "slider",
					"numinlets" : 1,
					"numoutlets" : 1,
					"orientation" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 138.0, 437.0, 147.0, 18.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-67",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 438.0, 65.0, 19.0 ],
					"style" : "",
					"text" : "mod 51 $1 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-68",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 140.0, 410.0, 231.0, 17.0 ],
					"style" : "",
					"text" : "MIDI pitch bend wheel range in semitones with channel"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-69",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 79.0, 409.0, 59.0, 19.0 ],
					"style" : "",
					"text" : "wheel 12 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-70",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 409.0, 53.0, 19.0 ],
					"style" : "",
					"text" : "wheel 2 1"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"format" : 6,
					"htricolor" : [ 0.87, 0.82, 0.24, 1.0 ],
					"id" : "obj-71",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 79.0, 388.0, 45.0, 19.0 ],
					"style" : "",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"tricolor" : [ 0.75, 0.75, 0.75, 1.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-72",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 272.0, 389.0, 356.0, 17.0 ],
					"style" : "",
					"text" : "MIDI pitch bend with value (use float argument to use LSB of 14-bit value) and channel"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-73",
					"maxclass" : "slider",
					"min" : 64.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"orientation" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 126.0, 388.0, 147.0, 18.0 ],
					"size" : 64.0,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-74",
					"maxclass" : "slider",
					"numinlets" : 1,
					"numoutlets" : 1,
					"orientation" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 106.0, 367.0, 147.0, 18.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-75",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 388.0, 53.0, 19.0 ],
					"style" : "",
					"text" : "bend $1 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-76",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 59.0, 277.0, 218.0, 17.0 ],
					"style" : "",
					"text" : "stop sound and reset all MIDI controllers and effects"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-77",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 71.0, 317.0, 19.0, 19.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-78",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 317.0, 45.0, 19.0 ],
					"style" : "",
					"text" : "mute $1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-79",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 92.0, 317.0, 33.0, 19.0 ],
					"style" : "",
					"text" : "mute"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-80",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 130.0, 317.0, 41.0, 19.0 ],
					"style" : "",
					"text" : "unmute"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-81",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 367.0, 80.0, 19.0 ],
					"style" : "",
					"text" : "control 91 $1 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-82",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 252.0, 368.0, 257.0, 17.0 ],
					"style" : "",
					"text" : "MIDI control change with contoller number, value and channel"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-83",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 523.0, 181.0, 99.0, 17.0 ],
					"style" : "",
					"text" : "print list of soundfonts"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-84",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 80.0, 176.0, 78.0, 17.0 ],
					"style" : "",
					"text" : "reload soundfont"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-85",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 175.0, 54.0, 19.0 ],
					"style" : "",
					"text" : "reload GM"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-86",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 154.0, 54.0, 19.0 ],
					"style" : "",
					"text" : "unload GM"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-87",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 147.0, 247.0, 238.0, 17.0 ],
					"style" : "",
					"text" : "select soundfont, bank and preset for given MIDI channel"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"htricolor" : [ 0.87, 0.82, 0.24, 1.0 ],
					"id" : "obj-88",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 111.0, 246.0, 35.0, 19.0 ],
					"style" : "",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"tricolor" : [ 0.75, 0.75, 0.75, 1.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-89",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 246.0, 85.0, 19.0 ],
					"style" : "",
					"text" : "select GM 0 $1 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-90",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 133.0, 226.0, 163.0, 17.0 ],
					"style" : "",
					"text" : "select program for given MIDI channel"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-91",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 275.0, 33.0, 19.0 ],
					"style" : "",
					"text" : "reset"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-92",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 296.0, 32.0, 19.0 ],
					"style" : "",
					"text" : "panic"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-93",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 439.0, 180.0, 83.0, 19.0 ],
					"style" : "",
					"text" : "print soundfonts"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-94",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 439.0, 243.0, 73.0, 19.0 ],
					"style" : "",
					"text" : "print channels"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-95",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 346.0, 73.0, 19.0 ],
					"style" : "",
					"text" : "note 69 127 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-96",
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 53.0, 693.0, 17.0, 53.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-97",
					"interpinlet" : 1,
					"maxclass" : "gain~",
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 27.0, 693.0, 17.0, 53.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"htricolor" : [ 0.87, 0.82, 0.24, 1.0 ],
					"id" : "obj-98",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 97.0, 225.0, 35.0, 19.0 ],
					"style" : "",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"tricolor" : [ 0.75, 0.75, 0.75, 1.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"htricolor" : [ 0.87, 0.82, 0.24, 1.0 ],
					"id" : "obj-99",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 79.0, 204.0, 35.0, 19.0 ],
					"style" : "",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"tricolor" : [ 0.75, 0.75, 0.75, 1.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-100",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 225.0, 71.0, 19.0 ],
					"style" : "",
					"text" : "program $1 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-101",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 204.0, 53.0, 19.0 ],
					"style" : "",
					"text" : "bank $1 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-102",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 99.0, 346.0, 61.0, 19.0 ],
					"style" : "",
					"text" : "note 69 0 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-103",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 24.0, 133.0, 62.0, 19.0 ],
					"style" : "",
					"text" : "load GM.sf2"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 18.0,
					"id" : "obj-104",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "signal", "signal", "anything" ],
					"patching_rect" : [ 24.0, 640.0, 122.0, 29.0 ],
					"style" : "",
					"text" : "fluidsynth~"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-105",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 148.0, 654.0, 377.0, 17.0 ],
					"style" : "",
					"text" : "fluidsynth~ [<polyphony (def: 256)> [<# of MIDI channels (def: 16)>]] [<soundfont filename>]"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-106",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 148.0, 641.0, 44.0, 17.0 ],
					"style" : "",
					"text" : "syntax:"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-107",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 115.0, 205.0, 149.0, 17.0 ],
					"style" : "",
					"text" : "select bank for given MIDI channel"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-108",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 80.0, 155.0, 151.0, 17.0 ],
					"style" : "",
					"text" : "remove soundfont or all soundfonts"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-109",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 513.0, 244.0, 218.0, 17.0 ],
					"style" : "",
					"text" : "print soundfont, bank and program for each channel"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-110",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 162.0, 347.0, 201.0, 17.0 ],
					"style" : "",
					"text" : "MIDI note on/off with pitch, velocity and channel"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-111",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 173.0, 318.0, 105.0, 17.0 ],
					"style" : "",
					"text" : "mute and unmute synth"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-112",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 58.0, 297.0, 267.0, 17.0 ],
					"style" : "",
					"text" : "stop sound and reset all MIDI controllers, modulators and effects"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 24.0,
					"id" : "obj-113",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 73.0, 7.0, 126.0, 33.0 ],
					"style" : "",
					"text" : "fluidsynth~"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-114",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 73.0, 40.0, 191.0, 20.0 ],
					"style" : "",
					"text" : "FluidSynth sound font synthesizer"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-115",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 118.0, 460.0, 106.0, 17.0 ],
					"style" : "",
					"text" : "linear overall gain factor"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-116",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 148.0, 712.0, 71.0, 17.0 ],
					"style" : "",
					"text" : "bang after load"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-117",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 170.0, 176.0, 198.0, 17.0 ],
					"style" : "",
					"text" : "(Note: Soundfonts can specified by name or id)"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-118",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 415.0, 745.0, 64.0, 17.0 ],
					"style" : "",
					"text" : "MIDI channel"
				}

			}
, 			{
				"box" : 				{
					"data" : [ 7765, "", "IBkSG0fBZn....PCIgDQRA...fC....SHX.....N0PWx....DLmPIQEBHf.B7g.YHB..dvPRDEDU3wY6alGabVd01+2LOy1yrOd799ZrSbvIwI3DGJkjBwsgPKMQ.EBPApHTjnRzpVJUTgRnsRfPEoJ9m1TTWTpZnJskTnpYgfSBARhytypiShi2sGuNdF6Ye446OL22LNEBseuu86iuO8dKMxVi8LOOm6y495bctNmG3+OeoKc5zZep+Qc5HYxjzc2cywN1wvpUq7k+xeYrYyFZZeper+O9RmNcxe2ue+b0qdUxKu7nrxJ6lafZZZnWud.vmOeL7vCSIkTBNc5DiFM9e967+EWISlD850ihhB.LxHi..1saGC2rOnllFoSmF850S94mO.XznQ4WzmWV50qGMMMzzzPmNc31saRjHAoRk5l6AAHc5znSmNzoSm72EegedYIBQEFo39Kc5zn+y5Cqnnfd85kdRMMMhGO9+wuo+2YkHQBo26F8l2TOX5zoAPFRJLRfOW4ASmNMoRkBCFLHcFZZZnnnbyMvDIRfQiFQmNcnooMaL8GEtloWEPtiEOdbLYxj78RlLI.XvfAzoSm76P72EeGJJJy46WbM+u5F4mYZhLu.YdQ0qWubCHSCTriJ98LgvybC4FWISl7lBd8+tF5MEEMyaLApDLq2HSuTlFAL2MFwlQ5zomCfUlaT.xPrOIi4F2n+uMCLYxjnoowLyLC82e+b0qdUb3vAETPATUUUgYylmiWSbCqWu94XzoRkB850iACFjmqSlLIoSmFSlLgNc5vfACepFw+UBSuoFnACFHVrXLxHivwO9wYW6ZW31sal27lGOxi7HTPAEHMNEEERlLIISlDe97wLyLC.3zoSxJqrPUUUZXFLX.iFMR5zoIZznnWudLa1LwhECEEkYAGtAn++iXfvrHnhc9QGcT762OZZZ32uexImbPQQQdCnnnP5zoo6t6lKdwKxDSLAd85kUtxUR4kWNNc5TZj5zoivgCy.CL.wiGGKVrPokVJ.yIBHSH++a2.Enbpppjat4hYylY5omlfACRnPgHVrXnSmNI3fllFwhEiAFX.16d2KW9xWlDIRP4kWN268dubO2y8Pt4lKFLXfYlYFN24NGu9q+5b7iebhEKF2y8bO7XO1iQyM2LVsZU5s+jVhbwBr.ANg39U7+bSMPc5zgISlH2bykhKtX73wCISlDiFMR3vgQSSSlFIUpTjLYRb3vAkTRIXylMBDH.yLyLDHP.hEKFACFj669tOTUUYfAFfCbfCPqs1JgBEhRJoD5niN3gdnGBMMM1vF1.uvK7BTVYk8I6Y9nbdhMAQXt.KPbt9lZfB.gLCQRlLIQhDQlyR.uKNCBPwEWLUVYkzVasQf.AHZznLv.CPe80GgBEBKVrfUqVwpUqDOdbTUUYYKaY7U+peUBFLHs1Zqr8sucLXv.Oxi7HzTSMIySJhVDnuBJjYxfQ7950q+lSUSfJlLYRlbxIYvAGjAFX.5niN3ZW6ZLyLyfhhBIRj..La1LZZZje94ypW8poolZRFpEHP.FYjQHTnPnooga2tYoKcoTXgERjHQHmbxg0st0wl1zl3se62libjiv67NuCszRK7rO6yxDSLg7LdznQkN.gAKPkyjZYxjIQYyadya4Sy.SkJEABDf1auc1yd1Cu669tDJTHlZpo3JW4Jnppx7m+7wtc6xyq5zoCiFMhSmNIPf.zc2cyzSOMVrXgBJn.V6ZWKkTRI3xkKJpnhX5omlqbkqP2c2MaZSaBmNcRznQo3hKl0st0gISl32869cbxSdRb5zIkUVYnppRpTolCwAwwjYlYFlbxIIZznyhObyLvXwhgOe9n81amicriQf.An5pqlryNatxUtBgBEhhJpHJszRknohP0ToRgOe93ZW6ZzWe8A.Ymc1rpUsJJnfBvnQinpphppJc2c2blybFVyZVCUWc0XvfAhFMJYmc1jWd4QvfAYG6XG3ymO73wiDvSP+SPIb5omlgGdXFXfAPud83xkKTd9m+42RxjIkgYhv.X1hb28t2M+o+zehgGdX1zl1DaZSahVZoEpppp3xW9xzZqshhhBKaYKi.ABHSbGLXPt7kuLs2d6zc2cid85Ib3vzTSMQkUVIpppnooQokVJEVXgr28tWFXfAnzRKkBJn.LYxDJJJX2tcV1xVF6cu6kScpSQGczAgBEhEsnEIiVhGONgBEhQFYD5t6tIVrXTXgEha2tQ44dtmaK.xaLXVR1SN4j7FuwavN24No2d6kErfEv29a+soppphhJpHZpolnwFajd5oG98+9eO5zoixJqLLa1LiN5nb7iebN3AOHs2d6LwDSPpTojoVxO+7k47RmNMd73Aa1rwV25VImbxg5qudb3vgLEjACFn2d6kqbkqPe80GQhDgku7kiSmNQQQASlLIAtb5zozyawhELzau8RznQwqWu3zoSLXv.ISljwFaL5pqtn6t6lfACR5zoImbxACFLfEKVHYxjzXiMxS7DOACN3f7K+k+R5u+94Vu0akToRwoO8o4rm8r32ue750KJJJLyLyvG7AefzqsfEr.YZmhKtXhEKFG8nGk0t10RN4jCFMZD850iMa1XdyadTPAEvTSME82e+LzPCw7l27jNE850ippJd85EMMMrXwxrfNyadyaKae6am1ZqMBFLHtc6lomdZ5u+94Tm5T3ymOhEKFVsZkMrgMPVYkkjvL.EUTQTUUUwN1wNXu6cubvCdPN3AOHevG7AzSO8fSmN49tu6ia61tMps1Z4RW5RbricLt5UuJoSmlJpnBBDH.W7hWjCcnCQxjIo7xKmZpoFYYThiMABDf95qOFe7wofBJfEu3EiUqVkHohTWVrXQB5o7c9NemszVaswd1yd3rm8rXxjIYUCwiGmolZJFZngXvAGjEtvER4kWtTWFQtFud8RjHQXfAFfwFaLlZpovjISL+4Oet669t4Iexmj0rl0vJW4Jo3hKlgGdXN3AOHs1ZqzSO8Pqs1Js1ZqLwDSPc0UGKaYKi5pqNra2tDxO6rylbxIGBDH.W6ZWiN5nCLYxD0We8X2t84TGo.3wfACn75u9quEEEE74yGW+5WmvgCSpToHUpTDNbX762OSM0TDHP.b4xE0Vasjc1YOmDqFLXfwFaL5omdXrwFizoSic61ot5pia61tMV4JWIYkUVX1rYpnhJvpUq3ymO5omdXngFhqe8qiOe9PUUklatYt8a+1ofBJPR4BlUrqbyMWTUUY5oml1auc74yG4jSNTWc0IYTIxeKpfQ4ke4WdKye9ymu3W7Kx7m+7YfAFfAGbPN+4OOW4JWgQGcTYRzgGdXb5zIKdwKVl6QTE+jSNIiN5n3ymORlLId73gZpoFV9xWNKZQKRVumISln7xKm4Mu4ghhBG8nGkPgBQhDInkVZgMrgMPiM1HNc5Dc5zQznQIb3vXznQLYxDUUUUzbyMia2t4pW8pr0stUps1ZIu7xSlONd73jHQBLYxDFzzzjIrqs1ZY8qe8DJTHlXhIXu6curqcsKIxU94mOgBEht5pKpu95mSE3lLYBiFMJCuiFMJSLwDxMHAKGSlLQN4jCqXEqfToRIiNLYxDqcsqkErfEfGOdPQQQRpHRjHDMZTrYyFpppTVYkw266883du26kt5pK5pqt3nG8nTVYkga2tk7dc3vAFD0iYvfALXv.Ymc1jc1YSgEVHKYIKgVZoE18t2Mm7jmDqVsR0UWMd73YNgCFLX.Od7fa2tkHrQhDgwGebFXfAX5omVd1VvczrYyTZokRs0VK986G61sSs0VqD8LUpTDMZTBEJDgBEZNbgMXv.1samFZnAl27lGCMzPRPEMMMhFMJIRjfDIRfAwg3DIRLG8OEmwV5RWJKcoKk.ABfllFlMaV5wxjMeEUTAKdwKl96ue74yGwiGmwFaLt90uNiN5n3xkKoGTHegppJyadyifAChKWtH+7yGiFMRhDIjrgDafBtk50qmnQixTSMEFMZDiFMR94mOVrXYNxeDIRjY+rha3nQiRf.AHb3vnppRN4jybpj1kKWxcvXwhQrXwHUpTRgmra2NqZUqhRKsTzoSGc0UWDNbXhFMJ974ihJpHrZ0pjwjHuka2tQmNc3zoSra2tLTWDUYylM73wCpppyQNDA48ImbRppppHmbxAKVrHq3vtc6ydtW.0KxmDJTHlYlYvnQi3vgCoQJJ7LXvfLxHiPe80GwhEC.750KKbgKDmNcx7m+7YiabibgKbAFd3gQUUUB2KVhaTiFMJq1vkKWyBqeCJqYxjIb3vgz.EjrEQDBNvBPkLWZZZerrgYJKeO8zCCLv.Tc0UKY2jHQBrYyFQhDg27MeS9w+3eLgCGl3wiiCGNn4lalG3Ad.dvG7AQud8DKVLhFMpTdBUUUrZ0p75jNcZBGNLCO7vxvUA8p3wiS3vgYrwFS58rYyFwiGWdVLd73XylMY3tvPSmNszYjHQBzkJUJsImbRN24NGQhDgUspUI8jm3Dm.2tcKAVRmNMSM0Tru8sOd5m9okgQ50qmHQhfppJEWbw7y9Y+LZt4lkelL8dYdCIzkIS4GzoSGCLv.btycNZu81I2bykUspUQ80WO50qmyblyPu81KNc5jpqtZIm1LWwiGWx.R4kdoWZKIRjPpZl.F2iGO32ueFd3gYxImDc5zgYylYngFhidzixgNzgPQQAud8Rs0VKkVZojLYRFZng3XG6XL93iiKWtviGOxyMhpTDFoHLU7Bf95qON5QOJ6ae6i8t28RO8zCgBEh63NtComc+6e+rqcsKfYUOP3IEfPYBTpr4Mu4sX1rYBFLHu+6+97lu4ahYylojRJg4O+4iUqVYxImjN5nCN6YOKG4HGg25sdKFZngPmNc3vgCpt5p4AevGjku7kSUUUEm4Lmgcu6cye4u7Wns1ZiHQhvTSMEIRjfYlYF4EWJqPF.GG7fGje8u9Wyd1ydXlYlgAGbP1+92OISljku7kiWudYjQFgsu8syt10tvue+rhUrBIFRlQGoSmFkW3EdgsnWudJu7xImbxg268dOFd3gwnQirvEtPxImblihZoSmlXwhwLyLCSM0TL93iSf.AngFZfFZnApu95YgKbg31saFd3g4Dm3Db3CeXZqs13PG5P3ymOFarwjUQnppJkg3.G3.7m+y+YN8oOMM0TS75u9qyi8XOF28ce2r4MuY1111Fesu1WiUu5UKSCckqbExO+7I+7yGa1rMmV6onnLqGLd73XznQJpnhHZznnppR4kWNkWd4XxjILa1LlLYBMMMb4xEtc6Vx8axImjImbRrZ0J4kWdjc1YSEUTgD1NRjHLxHiv3iONCMzP32ueld5oIUpTxiCBccN0oNEG9vGlImbRZt4l4YdlmAud8RkUVIszRKrsssMdq25sX8qe8rfEr.xJqrvkKWjUVYQAET.1saeNUVnnnfAg6zue+XvfA9A+fe.iM1XXxjIIK8zoSiEKVnvBKDGNbP1YmM0Vaszc2cygNzg3.G3.ryctSlXhIjkFUYkUhWudYQKZQricrC5niNjBBOxHivPCMDgBEhxJqL73wyb7lwiGGWtbgISl3Dm3DbtycN9JekuBc0UW7xu7KyxW9xo81amG+webIikLkuTQQAiFMRjHQPYKaYKaQud8bhSbBdsW60vmOerfEr.ovNACFjd6sWtzktDW5RWB+98SxjIkUMKpO7hW7hLzPCQO8zirhCud8Rt4lqT2Fa1rQO8zC986m.ABfNc5XQKZQje94ihhBtb4RFV6zoSdnG5g3zm9zrsssM5pqtvfAC7s9VeKty67N41u8amt5pK73wCEWbwRk65omdnmd5A850SVYkEJu3K9haQud8TbwEy4N24XqacqDNbXRjHAiO93zYmcxoO8o4HG4HbjibD5t6tYvAGTxpQmNcDIRDtzktDSM0TDNbXJnfBHqrxBGNbfYylwsa230qWTUUY7wGmolZJld5oAfFarQpnhJPQQAGNbP5zo4Dm3Db0qdU1zl1Dpppb3CeXhDIBETPAXwhEV7hWL974ieyu42Pe80GFLXf96uetzktDG8nGk25sdKlXhIn7xKGhGOtVvfA0zzzzBFLn1cbG2gFflYyl0b61slpppld850.juTTTzpqt5zV0pVk123a7Mz1vF1fVYkUllUqV0TUU09hewun1q7Juh1AO3A0t10tllOe9zlYlYz74ym1e+u+20d5m9o0prxJ0LZzn1i9nOpVas0lVjHQzBGNr1fCNn1u3W7Kzppppzd0W8U0RmNs1t10tz1xV1h167NuiVu81q1TSMkVhDIz1912tVd4kmld850LYxjlhhhlYylk2mKXAKPS4m7S9IawrYyDIRDLYxDkTRIDLXPlXhIHc5zRo3uka4VXQKZQTTQEgGOdnyN6TVpRu81KSLwDRBx4kWdTXgERokVJ4me9npphYylQQQg4Mu4wW3K7En95qmPgBw67NuCCMzP3wiGpt5pkonFczQYaaaajUVYwW+q+0IYxjTbwEiSmNwhEKXwhEpnhJngFZft5pKTTTHmbxgpppJty67NYsqcsTWc0gAACDUUUhGONUVYkrjkrDFczQIVrXRTrZpoFJqrxj0ns+8ue5niNX7wGW1BLUUUzoSGYmc1RYAE8uPbdVmNc3xkKZokVH2bykd6sWN7gOLIRjfpppJb4xEoRkh4O+4yV25V4u829azRKsvRVxRHXvfRvHAymkrjkvccW2EW+5WGGNbPokVJKdwKlxJqrYKWxjISy1ElOpZaAcqbyMW.njRJgFarQV3BWHkUVYXxjIRlLI24cdmzUWcIE1cfAFPh7t3EuXYdUQSPhFMJQiFkye9yS3vgIqrxhJpnBZs0VYaaaa7C+g+PV25VGszRKTc0USmc1I5zoiSdxSxt10t3a9M+l3vgCY05BNs4me9rl0rF5ryNIQhDTVYkwRVxRvoSmyJNrfylfnpWudIu7xCMMM5s2dIYxjjat4RAET.EWbwXxjIozAkTRIrhUrBhDIhDzPjuznQiRsJEP1ISlj1auc9vO7CYvAGjrxJKd3G9gY4Ke4b9yed99e+uO+w+3ejYlYFYDfSmNk0cZznQrYyFJJJDOdbYshkVZoR0ADLZDUlnr4Mu4sHJzUj7Md73zWe8w4O+4IQhD3vgCogKZ0rfsfISlvlMa3zoSb4xk7kpppTZcQYNZZZb8qec5ryN45W+5zUWcQmc1ISO8zje94yi9nOJ0UWcxBuangFX0qd0rl0rl438twRpDMIRQQAylMSVYkEVrXY1umDIRnIn0jYqnuvEt.+nezOhgFZHxM2boolZh65ttKJrvBQUUUVOlrObejgK.ZDU7KjdPn97oN0o3u9W+q7O9G+C5u+9Ib3vRYAe1m8Y4gdnGhJpnBBEJjrn0HQhvYNyYj7eqpppXYKaY.vLyLiTEsHQhPhDIjkXooogNsLJaOS41zoSGs1ZqbgKbAlZpovkKWrfEr.750qTcawAdGNbfEKVvue+L1Xiw3iON1samBKrP750KgCGlYlYFRjHAkTRILwDSv11113W8q9Uzc2cK2bLXv.0TSMrxUtRtka4VH+7yW5kO4IOICO7vDOdbVzhVD+7e9OmFarQoFnBFWhAaPrjM.MSF9ISlDSlLwJVwJnrxJC+98S5zokPzwiGeNC3iPG0PgBw3iONm9zmFKVrPc0UGKbgKTpk4niNJgCGlZpoF13F2HgBEhW60dMRkJEEVXg3wiGBDH.G3.Gf1aucrYyFiM1XLzPCIktLVrXxv7RKsT4FtPMuLCe0zzvf3OjY8ZhvTKVrPM0TizyJXujHQBhDIBgBE5i2o9ndZ7ge3Gxa+1uMACFjJqrRV+5WO2+8e+nppxniNJ6d26VlK7YdlmgvgCy1291whEKrpUsJb61MiO93zQGcP6s2tjZXwEWLM0TSzTSMwBW3Bo3hKlfAChYylkUQH3UOmN8FOdbMQWQyLLMUpTbkqbEFd3gYpolRNID4jSNjSN4PEUTgDcTTE8gNzg3Mdi2fVasUY4U1same6u82xW5K8kX3gGle5O8mxfCNH0TSM7.OvCvJVwJXe6aericrCFczQolZpQVUyoN0o3ZW6ZL7vCSKszB2+8e+zbyMSVYk0rsF6ibDYVa4MB9H4hdiiogllFczQGbhSbBN3AOHu+6+9b7iebYdO61sKOGlJUJFXfAX+6e+ru8su4PFWHPUt4lK2wcbGzYmcRO8zCCO7vDJTHZt4lolZpg3wiyG9geHiLxHxAbHUpTxp3EX.0We8xwQQDZJ7beRSbkxK8RuzVtQ2Zlc0QTU8Uu5U4jm7jb9yed9fO3Cn81amqbkqvXiMFiM1XzZqsxa+1uMW8pWka61tMdpm5oXiabir90ud1yd1C+g+vefnQixl1zlvlMaL4jShe+9ovBKjZpoFppppPud8L93iyfCNH986eNsFPvXxsa2TbwEiCGNjFQlhPmowALa4RYNKXY1gFGNbPwEWL0TSMTe80SgEVnTduKbgKv4N24niN5fN5nCN4IOIczQGXwhE1zl1D228cervEtPps1Z49u+6mxKub1111FFLXfm3IdBV8pWM1rYiryNarZ0Jd73QNnQwhESJFsXy1mOeb4KeYY+Qpt5pkcdJSGTlSDhNc5l0.uQCSXrB2uYylwkKW30qWxO+7ovBKjgFZHYYOhz.gBEBUUUdxm7Io1ZqUJSnPuTud8xy8bOG.r5UuZpolZvpUqRQiN6YOKW5RWh96ueBFLn77UpToHVrXL8zSyjSNIFMZjFarQra2tLh6Sx.AvPlS.3+zAzLlMEAhZkUVIwiGmRKsTd+2+8os1ZiwGe74LRGtb4BqVsJQjSjHAVsZkMtwMB.uxq7JricrCdhm3I3we7Gm.ABfe+948du2iie7iyPCMjj4iPkaMMMra2NQiFkCdvCJm.Cw.xmowIHdnnnLauI9zVwhEiImbRFarwjUfaylMLZzHqbkqjErfEvsdq2JG9vGlSe5SyXiMFQhDg95qu4LNkYd13ge3Gla4VtE17l2LO+y+7ryctS750qbnEBFLH.RY3EiGlfhVlooxjwjHIu3ZJZDy+zjNI9CoSmlImbR5t6to6t6F+98iYylwqWuRBupppzPCMfEKVX5omlXwhwDSLAG6XGi0st0Q1YmsLRHyUCMz.u3K9hjNcZdu268j2fvrCSTt4lK4kWdxdYL0TSwTSMk7lOu7xibxIGrZ0pjbelixblQi5RkJ0+DUMw+3gNzgniN5fd6sWRkJEkWd430qWrXwBCN3fLxHiPxjIwtc6DIRDN8oOMm6bmCud8x2869c4wdrGCKVr..QhDQR.VHRTu81KO0S8TbhSbBYawKqrxnwFajkrjkvRW5RQQQA+98SznQkjsUUUYEqXEy9bQ7QD5yLZYNnoISlTC9XXUQXAfT+jXwhgACFlUDmLTgqs1ZictycxQO5QIqrxBCFLvPCMDABDf0rl0vq9puJMzPC.y1mfLuYDTCOwINA6ZW6hSe5SiNc5nlZpgFarQprxJohJp.61sKOeKT+Nd732zoQLSCzPl.L2HJZ1YmMtc6F3i6HjneD4kWdTTQEIGai8su8Mmwpp+96mN6ryYkM3iF.VQ6kyrRiZpoFV8pWMYkUVLxHiHulhNRItd.xM2abzm+jFXVo3uhzDYtDLatQDIw6KJKxjISTWc0wZVyZvpUqL7vCyfCNnrHz5qudJu7xkv4vGyYUb1zjIS3xkKpnhJvsa2je94iSmNQUUUp0on2E2X32MCfTl1Kd73ZeRyYsHL5Sak4SWhHcRmc1IW7hWjKcoKgCGNXoKcozPCMHaxoHrLy46VTnc73wIPf.DJTHobjFMZTJkhPH3+cetMzEIRDMAsra7C+Y8kjYKiybTGGarwHUpTx91m4HhINGI17xzKjLYRhGOtLsfnW6BoTrZ0pDz5e0QcVWnPgzDggYZf+qXbwhESdFQPqRDBlYe9Ezsxj4uHT+FeTCxzfCEJDACFjvgCiACFvoSmXylMYAxhJHtY2q5hGOtl3e7FYz7YEhBHSqH7LhI.Nya1LePQxzykIGxLYBkYcoBZZgBERJ6QlZ8HbHepFXl4A+2w.EC1i3LjvftwKpPg.f4T4clF4mz2sPBCX1zBhHhLeIV2TC7l8n8HBCD+TL4QSO8zxC8eZetOurtoyrcl4DE+Tn24d1ydn6t6F.oWQLDNedZ8uzytTlnUhw2WL4stc6FOd7.Lqg94oG8U3yv.uwvSw6IxG8tu66RpTo3ttq6R9Hvl4SBymGVelO4KvGC9Hl0L850yi7HOBG8nGEUUU4.AIjW3ySdwOSPlLK2QLFVBzyIlXB4z.KlJpOMjw+u05y7gT9+We8Y9PJ++r9b95+Efk6HQCa2uDiB....PRE4DQtJDXBB" ],
					"embed" : 1,
					"id" : "obj-119",
					"maxclass" : "fpic",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "jit_matrix" ],
					"patching_rect" : [ 10.0, 7.0, 56.0, 76.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-100", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-101", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-102", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-103", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-30", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-104", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-96", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 85.0, 679.0, 59.5, 679.0 ],
					"source" : [ "obj-104", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-97", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-104", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-12", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-12", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-12", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-13", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-16", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-17", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-18", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-20", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 2 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-3", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-3", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-29", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-30", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-59", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-30", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-32", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-33", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-34", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-35", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-36", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-38", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 1 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-4", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-40", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-41", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-42", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-43", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-44", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-46", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-47", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-49", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-97", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-5", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-51", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-52", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-54", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-57", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-56", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-57", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-58", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-61", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-64", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-67", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-65", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-65", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-66", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-67", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-69", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-70", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-75", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-71", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-71", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-73", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-81", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-74", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-75", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-78", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-77", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-78", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-79", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-80", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-81", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-85", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-86", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-89", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-88", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-89", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-91", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-92", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-93", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-94", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-95", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-96", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-97", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-96", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-97", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-100", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-98", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-101", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-99", 0 ]
				}

			}
 ],
		"dependency_cache" : [ 			{
				"name" : "fluidsynth~.mxo",
				"type" : "iLaX"
			}
 ],
		"embedsnapshot" : 0
	}

}
