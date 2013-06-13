//
//  AvidaEDController.h
//  avida/apps/viewer-macos
//
//  Created by David on 11/30/10.
//  Copyright 2010-2012 Michigan State University. All rights reserved.
//  http://avida.devosoft.org/viewer-macos
//
//  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
//  following conditions are met:
//  
//  1.  Redistributions of source code must retain the above copyright notice, this list of conditions and the
//      following disclaimer.
//  2.  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//      following disclaimer in the documentation and/or other materials provided with the distribution.
//  3.  Neither the name of Michigan State University, nor the names of contributors may be used to endorse or promote
//      products derived from this software without specific prior written permission.
//  
//  THIS SOFTWARE IS PROVIDED BY MICHIGAN STATE UNIVERSITY AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
//  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL MICHIGAN STATE UNIVERSITY OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
//  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
//  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Authors: David M. Bryson <david@programerror.com>
//

#import <Cocoa/Cocoa.h>
#import <CorePlot/CorePlot.h>

#import "AvidaEDAnalyzePopulation.h"
#import "DraggableImageView.h"
#import "DroppableGraphHostingView.h"
#import "MapGridView.h"
#import "ViewerListener.h"


@class AvidaAppDelegate;
@class AvidaRun;
@class FlipView;
@class MapScaleView;

@class AvidaEDPopViewStatView;


@interface AvidaEDController : NSWindowController <DraggableImageViewDelegate, DropDelegate, MapDragDelegate, ViewerListener,
                                                   NSDraggingSource, NSSplitViewDelegate, NSWindowDelegate,
                                                   NSOutlineViewDelegate, NSOutlineViewDataSource>
{
  
  IBOutlet NSSplitView* mainSplitView;
  IBOutlet NSView* mainSplitViewLeft;

  // Freezer
  // --------------------------------------------------------------------------------------------------------------  
  NSURL* freezerURL;
  Avida::Viewer::FreezerPtr freezer;
  NSMutableArray* freezerConfigs;
  NSMutableArray* freezerGenomes;
  NSMutableArray* freezerWorlds;
  IBOutlet NSPathControl* pathWorkspace;
  IBOutlet NSOutlineView* outlineFreezer;
  
  // Population View
  // --------------------------------------------------------------------------------------------------------------  
  IBOutlet NSView* popView;
  IBOutlet NSSplitView* popSplitView;
  IBOutlet NSButton* btnMapSettingsFlip;
  IBOutlet FlipView* mapFlipView;

  IBOutlet NSButton* btnRunState;
  IBOutlet NSTextField* txtRun;
  IBOutlet NSTextField* txtUpdate;

  IBOutlet NSButton* btnPopView;
  IBOutlet NSButton* btnOrgView;
  IBOutlet NSButton* btnAnalyzeView;

  
  IBOutlet NSView* popViewDishView;
  IBOutlet NSScrollView* mapScrollView;
  IBOutlet MapGridView* mapView;
  IBOutlet MapScaleView* mapScaleView;
  IBOutlet NSPopUpButton* mapViewMode;
  IBOutlet NSSlider* mapZoom;
  
  IBOutlet AvidaEDPopViewStatView* popViewStatView;
  IBOutlet NSButton* btnTogglePopViewStatView;
  CGFloat lastPopViewStatViewWidth;
  BOOL popSplitViewIsAnimating;

  // Config View
  // --------------------------------------------------------------------------------------------------------------  
  IBOutlet NSSlider* sldCfgMutRate;
  IBOutlet NSTextField* txtCfgMutRate;
  IBOutlet NSTextField* txtCfgWorldX;
  IBOutlet NSTextField* txtCfgWorldY;
  IBOutlet NSMatrix* matCfgPlacement;
  IBOutlet NSMatrix* matCfgEnv;
  IBOutlet NSMatrix* matCfgRepeatability;
  IBOutlet NSMatrix* matCfgPauseAt;
  IBOutlet NSTextField* txtCfgPauseAt;
  IBOutlet NSStepper* stpCfgPauseAt;

  
  // Analyze View
  // --------------------------------------------------------------------------------------------------------------  
  IBOutlet NSView* analyzeView;
  NSMutableArray* analyzePops;
  IBOutlet NSArrayController* arrctlrAnalyze;
  IBOutlet NSPopUpButton* btnAnalyzeGraphSelectLeft;
  IBOutlet NSPopUpButton* btnAnalyzeGraphSelectRight;
  IBOutlet CPTGraphHostingView* graphViewAnalyze;
  CPTXYGraph* graphAnalyze;
  CPTXYPlotSpace* analyzePrimaryPlotSpace;
  CPTXYPlotSpace* analyzeSecondaryPlotSpace;
  CPTXYAxis* analyzeSecondaryYAxis;
  
  
  // Organism View
  // --------------------------------------------------------------------------------------------------------------  
  IBOutlet NSView* orgView;
  

  AvidaAppDelegate* app;
  
  AvidaRun* currentRun;
  Avida::Viewer::Listener* listener;
  Avida::Viewer::Map* map;
  Apto::Map<NSInteger, int> map_mode_to_color;
  BOOL runActive;
}

// Init and Dealloc Methods
- (id) initWithAppDelegate:(AvidaAppDelegate*)delegate;
- (id) initWithAppDelegate:(AvidaAppDelegate*)delegate inWorkspace:(NSURL*)dir;

- (void) dealloc;
- (void) finalize;


- (void) duplicateFreezerAtURL:(NSURL*)url;


// NSWindowController Methods
- (void) windowDidLoad;


// Actions
- (IBAction) toggleRunState:(id)sender;
- (IBAction) changeMapViewMode:(id)sender;
- (IBAction) changeMapZoom:(id)sender;
- (IBAction) togglePopViewStatView:(id)sender;
- (IBAction) toggleMapSettingsView:(id)sender;

- (IBAction) changeView:(id)sender;


- (IBAction) saveCurrentRun:(id)sender;
- (IBAction) saveCurrentConfig:(id)sender;
- (IBAction) saveSelectedOrganism:(id)sender;


- (IBAction) changeMutationRate:(id)sender;
- (IBAction) changeWorldSize:(id)sender;
- (IBAction) changePlacement:(id)sender;
- (IBAction) changeEnvironment:(id)sender;
- (IBAction) changeRepeatability:(id)sender;
- (IBAction) changePauseAt:(id)sender;

- (IBAction) changeAnalyzeGraphMode:(id)sender;
- (IBAction) changeAnalyzePopulationColor:(id)sender;
- (void) removeAnalyzePopulation:(id)pop;

- (void) envActionStateChange:(NSMutableDictionary*)newState;


@property (readonly) NSMutableArray* analyzePops;


// NSMenuValidation Informal Protocol
- (BOOL) validateMenuItem:(NSMenuItem*)item;


// NSSplitViewDelegate Protocol
- (void) splitView:(NSSplitView*)splitView resizeSubviewsWithOldSize:(NSSize)oldSize;
- (BOOL) splitView:(NSSplitView*)splitView canCollapseSubview:(NSView*)subview;
- (CGFloat) splitView:(NSSplitView*)splitView constrainMaxCoordinate:(CGFloat)proposedMax ofSubviewAt:(NSInteger)index;
- (CGFloat) splitView:(NSSplitView*)splitView constrainMinCoordinate:(CGFloat)proposedMin ofSubviewAt:(NSInteger)index;
- (void) splitViewDidResizeSubviews:(NSNotification*)notification;

// NSWindowDelegate Protocol
- (void) windowWillClose:(NSNotification*)notification;


// NSOutlineViewDelegate Protocol
- (BOOL)outlineView:(NSOutlineView*)outlineView shouldEditTableColumn:(NSTableColumn*)tableColumn item:(id)item;
- (BOOL)outlineView:(NSOutlineView*)outlineView shouldSelectItem:(id)item;

// NSOutlineViewDataSource Protocol
- (BOOL) outlineView:(NSOutlineView*)outlineView acceptDrop:(id <NSDraggingInfo>)info item:(id)item childIndex:(NSInteger)index;
- (NSDragOperation) outlineView:(NSOutlineView*)outlineView validateDrop:(id<NSDraggingInfo>)info proposedItem:(id)item proposedChildIndex:(NSInteger)index;
- (id) outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item;
- (BOOL) outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item;
- (NSInteger) outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item;
- (id) outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn*)tableColumn byItem:(id)item;
- (void) outlineView:(NSOutlineView*)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn byItem:(id)item;
- (BOOL) outlineView:(NSOutlineView*)outlineView writeItems:(NSArray*)items toPasteboard:(NSPasteboard*)pboard;

// FreezerOutlineViewDataSource Informal Protocol
- (void) outlineViewDidReceiveDeleteKey:(NSOutlineView*)outlineView;
- (void) outlineViewDidReceiveEnterOrSpaceKey:(NSOutlineView*)outlineView;


// MapDragDelegate
- (void) mapView:(MapGridView*)map handleDraggedConfig:(Avida::Viewer::FreezerID)fid;
- (void) mapView:(MapGridView*)map handleDraggedFreezerGenome:(Avida::Viewer::FreezerID)fid atX:(int)x Y:(int)y;
- (void) mapView:(MapGridView*)map handleDraggedGenome:(Genome*)fid atX:(int)x Y:(int)y;
- (void) mapView:(MapGridView*)map handleDraggedWorld:(Avida::Viewer::FreezerID)fid;


// NSDraggingSource
- (NSDragOperation) draggingSession:(NSDraggingSession*)session sourceOperationMaskForDraggingContext:(NSDraggingContext)context;
- (BOOL) ignoreModifierKeysForDraggingSession:(NSDraggingSession*)session;

// DraggableImageViewDelegate
- (void) draggableImageView:(DraggableImageView*)imageView writeToPasteboard:(NSPasteboard*)pboard;


// DropDelegate
- (NSDragOperation) draggingEnteredDestination:(id<NSDraggingDestination>)destination sender:(id<NSDraggingInfo>)sender;
- (NSDragOperation) draggingUpdatedForDestination:(id<NSDraggingDestination>)destination sender:(id<NSDraggingInfo>)sender;
- (BOOL) prepareForDragOperationForDestination:(id<NSDraggingDestination>)destination sender:(id<NSDraggingInfo>)sender;
- (BOOL) performDragOperationForDestination:(id<NSDraggingDestination>)destination sender:(id<NSDraggingInfo>)sender;


// Listener Methods
@property (readonly) Avida::Viewer::Listener* listener;

@property (readonly) MapGridView* mapView;

- (void) handleMap:(ViewerMap*)object;
- (void) handleUpdate:(ViewerUpdate*)object;


@end
