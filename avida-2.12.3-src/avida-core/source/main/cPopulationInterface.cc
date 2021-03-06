/*
 *  cPopulationInterface.cc
 *  Avida
 *
 *  Called "pop_interface.cc" prior to 12/5/05.
 *  Copyright 1999-2011 Michigan State University. All rights reserved.
 *  Copyright 1993-2003 California Institute of Technology.
 *
 *
 *  This file is part of Avida.
 *
 *  Avida is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 *  Avida is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along with Avida.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "cPopulationInterface.h"

#include "apto/platform.h"

#include "cDeme.h"
#include "cEnvironment.h"
#include "cHardwareManager.h"
#include "cOrganism.h"
#include "cOrgSinkMessage.h"
#include "cOrgMessage.h"
#include "cPopulation.h"
#include "cStats.h"
#include "cTestCPU.h"
#include "cRandom.h"
#include "cInstSet.h"

#include <cassert>
#include <algorithm>
#include <iterator>

#ifndef NULL
#define NULL 0
#endif

#if APTO_PLATFORM(WINDOWS)
namespace std
{
  /*inline __int64  abs(__int64 i) { return _abs64(i); }*/
}
#endif


cPopulationInterface::cPopulationInterface(cWorld* world) 
: m_world(world)
, m_cell_id(-1)
, m_deme_id(-1)
, m_av_cell_id(-1)
, m_av_facing(0)
, m_av_cell_faced(-1)
, m_prevseen_cell_id(-1)
, m_prev_task_cell(-1)
, m_num_task_cells(0)
, m_hgt_support(NULL)
{
}

cPopulationInterface::~cPopulationInterface() {
	if(m_hgt_support) {
		delete m_hgt_support;
	}
}

cOrganism* cPopulationInterface::GetOrganism() {
	return GetCell()->GetOrganism();
}

tSmartArray <cOrganism*> cPopulationInterface::GetLiveOrgList() {
  return m_world->GetPopulation().GetLiveOrgList();
}

cPopulationCell* cPopulationInterface::GetCell() { 
	return &m_world->GetPopulation().GetCell(m_cell_id);
}

cPopulationCell* cPopulationInterface::GetCell(int cell_id) { 
	return &m_world->GetPopulation().GetCell(cell_id);
}

cPopulationCell* cPopulationInterface::GetAVCell() { 
	return &m_world->GetPopulation().GetCell(m_av_cell_id);
}

int cPopulationInterface::GetAVCellID()
{
	return m_av_cell_id;
}

cPopulationCell* cPopulationInterface::GetCellFaced() {
	return &GetCell()->GetCellFaced();
}

cDeme* cPopulationInterface::GetDeme() {
  return &m_world->GetPopulation().GetDeme(m_deme_id);
}

int cPopulationInterface::GetCellData() {
  m_world->GetPopulation().GetCell(m_cell_id).UpdateCellDataExpired();
  return m_world->GetPopulation().GetCell(m_cell_id).GetCellData();
}

int cPopulationInterface::GetCellDataOrgID() {
  m_world->GetPopulation().GetCell(m_cell_id).UpdateCellDataExpired();
  return m_world->GetPopulation().GetCell(m_cell_id).GetCellDataOrgID();
}

int cPopulationInterface::GetCellDataUpdate() {
  m_world->GetPopulation().GetCell(m_cell_id).UpdateCellDataExpired();
  return m_world->GetPopulation().GetCell(m_cell_id).GetCellDataUpdate();
}

int cPopulationInterface::GetCellDataTerritory() {
  m_world->GetPopulation().GetCell(m_cell_id).UpdateCellDataExpired();
  return m_world->GetPopulation().GetCell(m_cell_id).GetCellDataTerritory();
}

int cPopulationInterface::GetCellDataForagerType() {
  m_world->GetPopulation().GetCell(m_cell_id).UpdateCellDataExpired();
  return m_world->GetPopulation().GetCell(m_cell_id).GetCellDataForagerType();
}

int cPopulationInterface::GetFacedCellData() {
  return m_world->GetPopulation().GetCell(m_cell_id).GetCellFaced().GetCellData();
}

int cPopulationInterface::GetFacedCellDataOrgID() {
  return m_world->GetPopulation().GetCell(m_cell_id).GetCellFaced().GetCellDataOrgID();
}

int cPopulationInterface::GetFacedCellDataUpdate() {
  return m_world->GetPopulation().GetCell(m_cell_id).GetCellFaced().GetCellDataUpdate();
}

int cPopulationInterface::GetFacedCellDataTerritory() {
  return m_world->GetPopulation().GetCell(m_cell_id).GetCellFaced().GetCellDataTerritory();
}

int cPopulationInterface::GetFacedAVData() {
  return m_world->GetPopulation().GetCell(m_av_cell_faced).GetCellData();
}

int cPopulationInterface::GetFacedAVDataOrgID() {
  return m_world->GetPopulation().GetCell(m_av_cell_faced).GetCellDataOrgID();
}

int cPopulationInterface::GetFacedAVDataUpdate() {
  return m_world->GetPopulation().GetCell(m_av_cell_faced).GetCellDataUpdate();
}

int cPopulationInterface::GetFacedAVDataTerritory() {
  return m_world->GetPopulation().GetCell(m_av_cell_faced).GetCellDataTerritory();
}

void cPopulationInterface::SetCellData(const int newData) {
  cPopulationCell& cell = m_world->GetPopulation().GetCell(m_cell_id);
  cell.SetCellData(newData, cell.GetOrganism()->GetID());
}

void cPopulationInterface::SetAVCellData(const int newData, const int org_id) {
  cPopulationCell& cell = m_world->GetPopulation().GetCell(m_av_cell_id);
  cell.SetCellData(newData, org_id);
}

bool cPopulationInterface::Divide(cAvidaContext& ctx, cOrganism* parent, const Genome& offspring_genome)
{
  assert(parent != NULL);
  assert(m_world->GetPopulation().GetCell(m_cell_id).GetOrganism() == parent);
  return m_world->GetPopulation().ActivateOffspring(ctx, offspring_genome, parent);
}

cOrganism* cPopulationInterface::GetNeighbor()
{
  cPopulationCell& cell = m_world->GetPopulation().GetCell(m_cell_id);
  assert(cell.IsOccupied());
  
  return cell.ConnectionList().GetFirst()->GetOrganism();
}

cOrganism* cPopulationInterface::GetAVRandNeighbor()
{
  return m_world->GetPopulation().GetCell(m_av_cell_faced).GetRandAvatar();
}

cOrganism* cPopulationInterface::GetAVRandNeighborPrey()
{
  return m_world->GetPopulation().GetCell(m_av_cell_faced).GetRandAVPrey();
}

cOrganism* cPopulationInterface::GetAVRandNeighborPred()
{
  return m_world->GetPopulation().GetCell(m_av_cell_faced).GetRandAVPred();
}

tArray<cOrganism*> cPopulationInterface::GetAVNeighbors()
{
  return m_world->GetPopulation().GetCell(m_av_cell_faced).GetCellAvatars();
}

tArray<cOrganism*> cPopulationInterface::GetAVNeighborPrey()
{
  return m_world->GetPopulation().GetCell(m_av_cell_faced).GetCellAVPrey();
}

bool cPopulationInterface::IsNeighborCellOccupied() {
  cPopulationCell & cell = m_world->GetPopulation().GetCell(m_cell_id);
  return cell.ConnectionList().GetFirst()->IsOccupied();
}

bool cPopulationInterface::HasAVNeighbor()
{
  return m_world->GetPopulation().GetCell(m_av_cell_faced).HasAvatar();
}

bool cPopulationInterface::HasAVNeighborPrey()
{
  return m_world->GetPopulation().GetCell(m_av_cell_faced).HasAVPrey();
}

bool cPopulationInterface::HasAVNeighborPred()
{
  return m_world->GetPopulation().GetCell(m_av_cell_faced).HasAVPred();
}

int cPopulationInterface::GetNumNeighbors()
{
  cPopulationCell & cell = m_world->GetPopulation().GetCell(m_cell_id);
  assert(cell.IsOccupied());
  
  return cell.ConnectionList().GetSize();
}

int cPopulationInterface::GetAVNumNeighbors()
{
  cPopulationCell & cell = m_world->GetPopulation().GetCell(m_av_cell_id);
  assert(cell.HasAvatar());
  
  return cell.ConnectionList().GetSize();
}

void cPopulationInterface::GetNeighborhoodCellIDs(tArray<int>& list)
{
  cPopulationCell& cell = m_world->GetPopulation().GetCell(m_cell_id);
  assert(cell.IsOccupied());
  
  list.Resize(cell.ConnectionList().GetSize());
  tConstListIterator<cPopulationCell> it(cell.ConnectionList());
  int i = 0;
  while (it.Next() != NULL) list[i++] = it.Get()->GetID();
}

int cPopulationInterface::GetFacing()
{
	cPopulationCell& cell = m_world->GetPopulation().GetCell(m_cell_id);
	assert(cell.IsOccupied());
	return cell.GetFacing();
}

int cPopulationInterface::GetFacedCellID()
{
	cPopulationCell& cell = m_world->GetPopulation().GetCell(m_cell_id).GetCellFaced();
	return cell.GetID();
}

int cPopulationInterface::GetAVFacedCellID()
{
	return m_av_cell_faced;
}

int cPopulationInterface::GetFacedDir()
{
	cPopulationCell& cell = m_world->GetPopulation().GetCell(m_cell_id);
	assert(cell.IsOccupied());
	return cell.GetFacedDir();
}

int cPopulationInterface::GetAVFacedDir()
{
	return m_av_facing;
}

int cPopulationInterface::GetNeighborCellContents() {
  cPopulationCell & cell = m_world->GetPopulation().GetCell(m_cell_id);
  return cell.ConnectionList().GetFirst()->GetCellData();
}

void cPopulationInterface::Rotate(int direction)
{
  cPopulationCell & cell = m_world->GetPopulation().GetCell(m_cell_id);
  assert(cell.IsOccupied());
	
  if (direction >= 0) cell.ConnectionList().CircNext();
  else cell.ConnectionList().CircPrev();

  if (m_world->GetConfig().USE_AVATARS.Get()) {
    cPopulationCell & av_cell = m_world->GetPopulation().GetCell(m_av_cell_id);
    assert(av_cell.HasAvatar());
    int org_facing = av_cell.GetFacedDir();
    // rotate the avatar cell to match the direction of the true org cell
    for (int i = 0; i < av_cell.ConnectionList().GetSize(); i++) {
      av_cell.ConnectionList().CircNext();
      if (av_cell.GetFacedDir() == cell.GetFacedDir()) break;
    }
    // save the avatar facing and faced cell data for this org...we cannot rely on av_cell facing after this b/c other avatars could rotate same cell
    SetAvatarFacing(cell.GetFacedDir());
    SetAvatarFacedCell(av_cell.ConnectionList().GetFirst()->GetID());
    
    // now put the avatar cell back where it belongs in case there is a real org in the avatar cell
    for (int i = 0; i < av_cell.ConnectionList().GetSize(); i++) {
      av_cell.ConnectionList().CircNext();
      if (av_cell.GetFacedDir() == org_facing) break;
    }    
  }
}

int cPopulationInterface::GetInputAt(int& input_pointer)
{
  cPopulationCell& cell = m_world->GetPopulation().GetCell(m_cell_id);
  assert(cell.IsOccupied());
  return cell.GetInputAt(input_pointer);
}

void cPopulationInterface::ResetInputs(cAvidaContext& ctx) 
{ 
  m_world->GetPopulation().GetCell(m_cell_id).ResetInputs(ctx); 
}

const tArray<int>& cPopulationInterface::GetInputs() const
{
  return m_world->GetPopulation().GetCell(m_cell_id).GetInputs();
}

const tArray<double>& cPopulationInterface::GetResources(cAvidaContext& ctx) 
{
  return m_world->GetPopulation().GetCellResources(m_cell_id, ctx); 
}

const tArray<double>& cPopulationInterface::GetAVResources(cAvidaContext& ctx) 
{
  return m_world->GetPopulation().GetCellResources(m_av_cell_id, ctx); 
}

const tArray<double>& cPopulationInterface::GetFacedCellResources(cAvidaContext& ctx) 
{
  return m_world->GetPopulation().GetCellResources(GetCell()->GetCellFaced().GetID(), ctx); 
}

const tArray<double>& cPopulationInterface::GetFacedAVResources(cAvidaContext& ctx) 
{
  return m_world->GetPopulation().GetCellResources(m_world->GetPopulation().GetCell(m_av_cell_faced).GetID(), ctx); 
}

const tArray<double>& cPopulationInterface::GetCellResources(int cell_id, cAvidaContext& ctx) 
{
  return m_world->GetPopulation().GetCellResources(cell_id, ctx); 
}

const tArray<double>& cPopulationInterface::GetFrozenResources(cAvidaContext& ctx, int cell_id) 
{
  return m_world->GetPopulation().GetFrozenResources(ctx, cell_id); 
}

const tArray<double>& cPopulationInterface::GetDemeResources(int deme_id, cAvidaContext& ctx) 
{
  return m_world->GetPopulation().GetDemeCellResources(deme_id, m_cell_id, ctx); 
}

const tArray< tArray<int> >& cPopulationInterface::GetCellIdLists()
{
	return m_world->GetPopulation().GetCellIdLists();
}

int cPopulationInterface::GetCurrPeakX(cAvidaContext& ctx, int res_id) 
{ 
  return m_world->GetPopulation().GetCurrPeakX(ctx, res_id); 
} 

int cPopulationInterface::GetCurrPeakY(cAvidaContext& ctx, int res_id) 
{ 
  return m_world->GetPopulation().GetCurrPeakY(ctx, res_id); 
} 

int cPopulationInterface::GetFrozenPeakX(cAvidaContext& ctx, int res_id) 
{ 
  return m_world->GetPopulation().GetFrozenPeakX(ctx, res_id); 
} 

int cPopulationInterface::GetFrozenPeakY(cAvidaContext& ctx, int res_id) 
{ 
  return m_world->GetPopulation().GetFrozenPeakY(ctx, res_id); 
} 

void cPopulationInterface::TriggerDoUpdates(cAvidaContext& ctx)
{
  m_world->GetPopulation().TriggerDoUpdates(ctx);
}

void cPopulationInterface::UpdateResources(cAvidaContext& ctx, const tArray<double>& res_change)
{
  return m_world->GetPopulation().UpdateCellResources(ctx, res_change, m_cell_id);
}

void cPopulationInterface::UpdateAVResources(cAvidaContext& ctx, const tArray<double>& res_change)
{
  return m_world->GetPopulation().UpdateCellResources(ctx, res_change, m_av_cell_id);
}

void cPopulationInterface::UpdateDemeResources(cAvidaContext& ctx, const tArray<double>& res_change)
{
  return m_world->GetPopulation().UpdateDemeCellResources(ctx, res_change, m_cell_id);
}

void cPopulationInterface::Die(cAvidaContext& ctx) 
{
  cPopulationCell & cell = m_world->GetPopulation().GetCell(m_cell_id);
  m_world->GetPopulation().KillOrganism(cell, ctx);
}

void cPopulationInterface::KillCellID(int target, cAvidaContext& ctx) 
{
  cPopulationCell & cell = m_world->GetPopulation().GetCell(target);
  m_world->GetPopulation().KillOrganism(cell, ctx); 
}

void cPopulationInterface::Kaboom(int distance, cAvidaContext& ctx) 
{
  cPopulationCell & cell = m_world->GetPopulation().GetCell(m_cell_id);
  m_world->GetPopulation().Kaboom(cell, ctx, distance); 
}

void cPopulationInterface::SpawnDeme(cAvidaContext& ctx) 
{
  // const int num_demes = m_world->GetPopulation().GetNumDemes();
	
  // Spawn the current deme; no target ID will put it into a random deme.
  const int deme_id = m_world->GetPopulation().GetCell(m_cell_id).GetDemeID();
	
  m_world->GetPopulation().SpawnDeme(deme_id, ctx); 
}

cOrgSinkMessage* cPopulationInterface::NetReceive()
{
  cPopulationCell& cell = m_world->GetPopulation().GetCell(m_cell_id);
  assert(cell.IsOccupied());
  
  switch(m_world->GetConfig().NET_STYLE.Get())
  {
		case 1: // Receiver Facing
    {
      cOrganism* cur_neighbor = cell.ConnectionList().GetFirst()->GetOrganism();
      cOrgSinkMessage* msg = NULL;
      if (cur_neighbor != NULL && (msg = cur_neighbor->NetPop()) != NULL) return msg;
    }
			break;
			
		case 0: // Random Next - First Available
		default:
    {
      const int num_neighbors = cell.ConnectionList().GetSize();
      for (int i = 0; i < num_neighbors; i++) {
        cell.ConnectionList().CircNext();
        
        cOrganism* cur_neighbor = cell.ConnectionList().GetFirst()->GetOrganism();
        cOrgSinkMessage* msg = NULL;
        if (cur_neighbor != NULL && (msg = cur_neighbor->NetPop()) != NULL ) return msg;
      }
    }
			break;
  }
  
  return NULL;
}

bool cPopulationInterface::NetRemoteValidate(cAvidaContext& ctx, cOrgSinkMessage* msg)
{
  cOrganism* org = m_world->GetPopulation().GetCell(msg->GetSourceID()).GetOrganism();
  return (org != NULL && org->NetRemoteValidate(ctx, msg->GetOriginalValue()));
}

int cPopulationInterface::ReceiveValue()
{
  cPopulationCell & cell = m_world->GetPopulation().GetCell(m_cell_id);
  assert(cell.IsOccupied());
  
  const int num_neighbors = cell.ConnectionList().GetSize();
  for (int i = 0; i < num_neighbors; i++) {
    cell.ConnectionList().CircNext();
    
    cOrganism* cur_neighbor = cell.ConnectionList().GetFirst()->GetOrganism();
    if (cur_neighbor == NULL || cur_neighbor->GetSentActive() == false) {
      continue;
    }
    
    return cur_neighbor->RetrieveSentValue();
  }
  
  return 0;
}

void cPopulationInterface::SellValue(const int data, const int label, const int sell_price, const int org_id)
{
	m_world->GetPopulation().AddSellValue(data, label, sell_price, org_id, m_cell_id);
}

int cPopulationInterface::BuyValue(const int label, const int buy_price)
{
	int value = m_world->GetPopulation().BuyValue(label, buy_price, m_cell_id);
	return value;
}

bool cPopulationInterface::InjectParasite(cOrganism* host, cBioUnit* parent, const cString& label, const Sequence& injected_code)
{
  assert(parent != NULL);
  assert(m_world->GetPopulation().GetCell(m_cell_id).GetOrganism() == host);
  
  return m_world->GetPopulation().ActivateParasite(host, parent, label, injected_code);
}

bool cPopulationInterface::UpdateMerit(double new_merit)
{
  return m_world->GetPopulation().UpdateMerit(m_cell_id, new_merit);
}

bool cPopulationInterface::TestOnDivide()
{
  return m_world->GetTestOnDivide();
}


/*! Internal-use method to consolidate message-sending code.
 */
bool cPopulationInterface::SendMessage(cOrgMessage& msg, cPopulationCell& rcell) {
	
	bool dropped=false;
	bool lost=false;
	
	static const double drop_prob = m_world->GetConfig().NET_DROP_PROB.Get();
  if((drop_prob > 0.0) && m_world->GetRandom().P(drop_prob)) {
		// message dropped
		GetDeme()->messageDropped();
		GetDeme()->messageSendFailed();
		dropped = true;
	}
	
	// Fail if the cell we're facing is not occupied.
  if(!rcell.IsOccupied()) {
		GetDeme()->messageSendFailed();
		lost = true;
	}
	
	// record this message, regardless of whether it's actually received.
	if(m_world->GetConfig().NET_LOG_MESSAGES.Get()) {
		m_world->GetStats().LogMessage(msg, dropped, lost);
	}
	
	if(dropped || lost) {
		return false;
	}
	
	cOrganism* recvr = rcell.GetOrganism();
  assert(recvr != 0);
  recvr->ReceiveMessage(msg);
	m_world->GetStats().SentMessage(msg);
	GetDeme()->IncMessageSent();
	GetDeme()->MessageSuccessfullySent(); // No idea what the difference is here...
  return true;
}

bool cPopulationInterface::SendMessage(cOrgMessage& msg, int cellid) {
  cPopulationCell& cell = m_world->GetPopulation().GetCell(cellid);
	return SendMessage(msg, cell);	
}


/*! Send a message to the faced organism, failing if this cell does not have 
 neighbors or if the cell currently faced is not occupied.
 */
bool cPopulationInterface::SendMessage(cOrgMessage& msg) {
	cPopulationCell& cell = m_world->GetPopulation().GetCell(m_cell_id);
	assert(cell.IsOccupied()); // This organism; sanity.
	
  cPopulationCell* rcell = cell.ConnectionList().GetFirst();
  assert(rcell != 0); // Cells should never be null.	
	return SendMessage(msg, *rcell);
}


/*! Send a message to the faced organism, failing if this cell does not have 
 neighbors or if the cell currently faced is not occupied. */
bool cPopulationInterface::BroadcastMessage(cOrgMessage& msg, int depth) {
  cPopulationCell& cell = m_world->GetPopulation().GetCell(m_cell_id);
  assert(cell.IsOccupied()); // This organism; sanity.
	
	// Get the set of cells that are within range.
	std::set<cPopulationCell*> cell_set;
	cell.GetNeighboringCells(cell_set, depth);
	
	// Remove this cell from the set!
	cell_set.erase(&cell);
	
	// Now, send a message towards each cell:
	for(std::set<cPopulationCell*>::iterator i=cell_set.begin(); i!=cell_set.end(); ++i) {
		SendMessage(msg, **i);
	}
	return true;
}


bool cPopulationInterface::BcastAlarm(int jump_label, int bcast_range) {
  bool successfully_sent(false);
  cPopulationCell& scell = m_world->GetPopulation().GetCell(m_cell_id);
  assert(scell.IsOccupied()); // This organism; sanity.
	
  const int ALARM_SELF = m_world->GetConfig().ALARM_SELF.Get(); // does an alarm affect the sender; 0=no  non-0=yes
  
  if(bcast_range > 1) { // multi-hop messaging
    cDeme& deme = m_world->GetPopulation().GetDeme(GetDemeID());
    for(int i = 0; i < deme.GetSize(); i++) {
      int possible_receiver_id = deme.GetCellID(i);
      cPopulationCell& rcell = m_world->GetPopulation().GetCell(possible_receiver_id);
			
      if(rcell.IsOccupied() && possible_receiver_id != GetCellID()) {
        //check distance
        pair<int, int> sender_pos = deme.GetCellPosition(GetCellID());
        pair<int, int> possible_receiver_pos = deme.GetCellPosition(possible_receiver_id);
        int hop_distance = max( abs(sender_pos.first  - possible_receiver_pos.first),
															 abs(sender_pos.second - possible_receiver_pos.second));
        if(hop_distance <= bcast_range) {
          // send alarm to organisms
          cOrganism* recvr = rcell.GetOrganism();
          assert(recvr != NULL);
          recvr->moveIPtoAlarmLabel(jump_label);
          successfully_sent = true;
        }
      }
    }
  } else { // single hop messaging
    for(int i = 0; i < scell.ConnectionList().GetSize(); i++) {
      cPopulationCell* rcell = scell.ConnectionList().GetPos(i);
      assert(rcell != NULL); // Cells should never be null.
			
      // Fail if the cell we're facing is not occupied.
      if(!rcell->IsOccupied())
        continue;
      cOrganism* recvr = rcell->GetOrganism();
      assert(recvr != NULL);
      recvr->moveIPtoAlarmLabel(jump_label);
      successfully_sent = true;
    }
  }
  
  if(ALARM_SELF) {
    scell.GetOrganism()->moveIPtoAlarmLabel(jump_label);
  }
  return successfully_sent;
}

void cPopulationInterface::DivideOrgTestamentAmongDeme(double value){
  cDeme* deme = GetDeme();
  for(int i = 0; i < deme->GetSize(); i++) {
    cPopulationCell& cell = deme->GetCell(i);
    if(cell.IsOccupied()) {
      cOrganism* org = cell.GetOrganism();
      org->GetPhenotype().EnergyTestament(value/deme->GetOrgCount());
    }
  }
}

/*! Send a flash to all neighboring organisms. */
void cPopulationInterface::SendFlash() {
  cPopulationCell& cell = m_world->GetPopulation().GetCell(m_cell_id);
  assert(cell.IsOccupied());
	
  for(int i=0; i<cell.ConnectionList().GetSize(); ++i) {
    cPopulationCell* neighbor = cell.ConnectionList().GetFirst();
    if(neighbor->IsOccupied()) {
      neighbor->GetOrganism()->ReceiveFlash();
    }
    cell.ConnectionList().CircNext();
  }
}

int cPopulationInterface::GetStateGridID(cAvidaContext& ctx)
{
  return ctx.GetRandom().GetUInt(m_world->GetEnvironment().GetNumStateGrids());
}

/* Rotate an organism to face the neighbor with the highest reputation */
void cPopulationInterface::RotateToGreatestReputation() 
{
	
	cPopulationCell& cell = m_world->GetPopulation().GetCell(GetCellID());
	int high_rep=-1; 
	vector <int> high_rep_orgs;
	
	
	// loop to find the max reputation
	for(int i=0; i<cell.ConnectionList().GetSize(); ++i) {
		const cPopulationCell* faced_cell = cell.ConnectionList().GetFirst();
		// cell->organism, if occupied, check reputation, etc.
		if (IsNeighborCellOccupied()) {
			cOrganism* cur_neighbor = faced_cell->GetOrganism();
			
			// if it has high reputation	
			if (cur_neighbor->GetReputation() >= high_rep) {
				if (cur_neighbor->GetReputation() > high_rep) {
					high_rep = cur_neighbor->GetReputation();
					high_rep_orgs.clear();
				}
				high_rep_orgs.push_back(cur_neighbor->GetID()); 
			} 
		}
		
		// check the next neighbor
		cell.ConnectionList().CircNext();
	}
	
	// Pick an organism to donate to
	
	if (high_rep_orgs.size() > 0) {
		unsigned int rand_num = m_world->GetRandom().GetUInt(0, high_rep_orgs.size()); 
		int high_org_id = high_rep_orgs[rand_num];
		
		for(int i=0; i<cell.ConnectionList().GetSize(); ++i) {
			const cPopulationCell* faced_cell = cell.ConnectionList().GetFirst();
			
			if (IsNeighborCellOccupied()) {
				
				cOrganism* cur_neighbor = faced_cell->GetOrganism();
				
				// if it has high reputation	
				if (cur_neighbor->GetID() == high_org_id) {
					break;
				}
			}
			
			cell.ConnectionList().CircNext();
			
		}
	}
	
}

/* Rotate an organism to face the neighbor with the highest reputation 
 where the neighbor has a different tag than the organism*/
void cPopulationInterface::RotateToGreatestReputationWithDifferentTag(int tag) 
{
	
	cPopulationCell& cell = m_world->GetPopulation().GetCell(GetCellID());
	int high_rep=-1; 
	vector <int> high_rep_orgs;
	
	// loop to find the max reputation
	for(int i=0; i<cell.ConnectionList().GetSize(); ++i) {
		const cPopulationCell* faced_cell = cell.ConnectionList().GetFirst();
		// cell->organism, if occupied, check reputation, etc.
		if (IsNeighborCellOccupied()) {
			cOrganism* cur_neighbor = faced_cell->GetOrganism();
			
			// if it has high reputation	
			if ((cur_neighbor->GetTagLabel() != tag) && (cur_neighbor->GetReputation() >= high_rep)) {
				if (cur_neighbor->GetReputation() > high_rep) {
					high_rep = cur_neighbor->GetReputation();
					high_rep_orgs.clear();
				}
				high_rep_orgs.push_back(cur_neighbor->GetID()); 
			} 
		}
		
		// check the next neighbor
		cell.ConnectionList().CircNext();
	}
	
	// Pick an organism to donate to
	
	if (high_rep_orgs.size() > 0) {
		unsigned int rand_num = m_world->GetRandom().GetUInt(0, high_rep_orgs.size()); 
		int high_org_id = high_rep_orgs[rand_num];
		
		for(int i=0; i<cell.ConnectionList().GetSize(); ++i) {
			const cPopulationCell* faced_cell = cell.ConnectionList().GetFirst();
			
			if (IsNeighborCellOccupied()) {
				
				cOrganism* cur_neighbor = faced_cell->GetOrganism();
				
				// if it has high reputation	
				if (cur_neighbor->GetID() == high_org_id) {
					break;
				}
			}
			
			cell.ConnectionList().CircNext();
			
		}
		
		
		
	}
	
}

/* Rotate an organism to face the neighbor with the highest reputation 
 where the neighbor has a different tag than the organism*/
void cPopulationInterface::RotateToGreatestReputationWithDifferentLineage(int line) 
{
	
	cPopulationCell& cell = m_world->GetPopulation().GetCell(GetCellID());
	int high_rep=-1; 
	vector <int> high_rep_orgs;
	
	// loop to find the max reputation
	for(int i=0; i<cell.ConnectionList().GetSize(); ++i) {
		const cPopulationCell* faced_cell = cell.ConnectionList().GetFirst();
		// cell->organism, if occupied, check reputation, etc.
		if (IsNeighborCellOccupied()) {
			cOrganism* cur_neighbor = faced_cell->GetOrganism();
			
			// if it has high reputation	
			if ((cur_neighbor->GetLineageLabel() != line) && (cur_neighbor->GetReputation() >= high_rep)) {
				if (cur_neighbor->GetReputation() > high_rep) {
					high_rep = cur_neighbor->GetReputation();
					high_rep_orgs.clear();
				}
				high_rep_orgs.push_back(cur_neighbor->GetID()); 
			} 
		}
		
		// check the next neighbor
		cell.ConnectionList().CircNext();
	}
	
	// Pick an organism to donate to
	
	if (high_rep_orgs.size() > 0) {
		unsigned int rand_num = m_world->GetRandom().GetUInt(0, high_rep_orgs.size()); 
		int high_org_id = high_rep_orgs[rand_num];
		
		for(int i=0; i<cell.ConnectionList().GetSize(); ++i) {
			const cPopulationCell* faced_cell = cell.ConnectionList().GetFirst();
			
			if (IsNeighborCellOccupied()) {
				
				cOrganism* cur_neighbor = faced_cell->GetOrganism();
				
				// if it has high reputation	
				if (cur_neighbor->GetID() == high_org_id) {
					break;
				}
			}
			
			cell.ConnectionList().CircNext();
			
		}
	}	
}

/*! Link this organism's cell to the cell it is currently facing.
 */
void cPopulationInterface::CreateLinkByFacing(double weight) {
	cDeme* deme = GetDeme(); assert(deme);
	cPopulationCell* this_cell = GetCell(); assert(this_cell);
	cPopulationCell* that_cell = GetCellFaced(); assert(that_cell);
	deme->GetNetwork().Connect(*this_cell, *that_cell, weight);
}

/*! Link this organism's cell to the cell with coordinates (x,y).
 */
void cPopulationInterface::CreateLinkByXY(int x, int y, double weight) {
	cDeme* deme = GetDeme(); assert(deme);
	cPopulationCell* this_cell = GetCell(); assert(this_cell);
	// the static casts here are to fix a problem with -2^31 being sent in as a 
	// cell coordinate.  the problem is that a 2s-complement int can hold a negative
	// number whose absolute value is too large for the int to hold.  when this happens,
	// abs returns the value unmodified.
	int cellx = std::abs(static_cast<long long int>(x)) % deme->GetWidth();
	int celly = std::abs(static_cast<long long int>(y)) % deme->GetHeight();
	assert(cellx >= 0);
	assert(cellx < deme->GetWidth());
	assert(celly >= 0);
	assert(celly < deme->GetHeight());
	deme->GetNetwork().Connect(*this_cell, deme->GetCell(cellx, celly), weight);
}

/*! Link this organism's cell to the cell with index idx.
 */
void cPopulationInterface::CreateLinkByIndex(int idx, double weight) {
	cDeme* deme = GetDeme(); assert(deme);
	cPopulationCell* this_cell = GetCell(); assert(this_cell);
	// the static casts here are to fix a problem with -2^31 being sent in as a 
	// cell coordinate.  the problem is that a 2s-complement int can hold a negative
	// number whose absolute value is too large for the int to hold.  when this happens,
	// abs returns the value unmodified.
	int that_cell = std::abs(static_cast<long long int>(idx)) % deme->GetSize();
	assert(that_cell >= 0);
	assert(that_cell < deme->GetSize());
	deme->GetNetwork().Connect(*this_cell, deme->GetCell(that_cell), weight);
}

/*! Broadcast a message to all organisms that are connected by this network.
 */
bool cPopulationInterface::NetworkBroadcast(cOrgMessage& msg) {	
	cDeme* deme = GetDeme(); assert(deme);
	cPopulationCell* this_cell = GetCell(); assert(this_cell);
	deme->GetNetwork().BroadcastToNeighbors(*this_cell, msg, this);
	return true;
}

/*! Unicast a message to the current selected organism.
 */
bool cPopulationInterface::NetworkUnicast(cOrgMessage& msg) {
	cDeme* deme = GetDeme(); assert(deme);
	cPopulationCell* this_cell = GetCell(); assert(this_cell);
	deme->GetNetwork().Unicast(*this_cell, msg, this);
	return true;
}

/*! Rotate to select a new network link.
 */
bool cPopulationInterface::NetworkRotate(int x) {
	cDeme* deme = GetDeme(); assert(deme);
	cPopulationCell* this_cell = GetCell(); assert(this_cell);
	deme->GetNetwork().Rotate(*this_cell, x);
	return true;
}

/*! Select a new network link.
 */
bool cPopulationInterface::NetworkSelect(int x) {
	cDeme* deme = GetDeme(); assert(deme);
	cPopulationCell* this_cell = GetCell(); assert(this_cell);
	deme->GetNetwork().Select(*this_cell, x);
	return true;
}

/*! Called when this individual is the donor organism during conjugation.
 
 This method causes this individual to "donate" a fragment of its own genome to
 another organism selected from the population.
 */
void cPopulationInterface::DoHGTDonation(cAvidaContext& ctx) {
	cPopulationCell* target=0;
	
	switch(m_world->GetConfig().HGT_CONJUGATION_METHOD.Get()) {
		case 0: { // selected at random from neighborhood
			std::set<cPopulationCell*> occupied_cell_set;
			GetCell()->GetOccupiedNeighboringCells(occupied_cell_set, 1);
			if(occupied_cell_set.size()==0) {
				// nothing to do here, there are no neighbors
				return;
			}
			std::set<cPopulationCell*>::iterator selected=occupied_cell_set.begin();
			std::advance(selected, ctx.GetRandom().GetInt(occupied_cell_set.size()));
			target = *selected;
			break;
		}
		case 1: { // faced individual
			target = GetCellFaced();
			if(!target->IsOccupied()) {
				// nothing to do, we're facing an empty cell.
				return;
			}
			break;
		}
		default: {
			m_world->GetDriver().RaiseFatalException(1, "HGT_CONJUGATION_METHOD is set to an invalid value.");
			break;
		}
	}
	assert(target != 0);
	fragment_list_type fragments;
	cGenomeUtil::RandomSplit(ctx, 
													 m_world->GetConfig().HGT_FRAGMENT_SIZE_MEAN.Get(),
													 m_world->GetConfig().HGT_FRAGMENT_SIZE_VARIANCE.Get(),
													 GetOrganism()->GetGenome().GetSequence(),
													 fragments);
	target->GetOrganism()->GetOrgInterface().ReceiveHGTDonation(fragments[ctx.GetRandom().GetInt(fragments.size())]);
}


/*! Called when this organism "requests" an HGT conjugation.
 
 Technically, organisms don't request an HGT conjugation.  However, this provides
 an alternative to population-level conjugational events.  Specifically, whenever
 an organism replicates, there is the possibility that its offspring conjugates
 with another organism in the population -- that is what we check here.
 
 This method is closely related to HGT donation, except here we're looking for
 the donatOR, instead of the donatEE.
 */
void cPopulationInterface::DoHGTConjugation(cAvidaContext& ctx) {
	cPopulationCell* source=0;
	
	switch(m_world->GetConfig().HGT_CONJUGATION_METHOD.Get()) {
		case 0: { // selected at random from neighborhood
			std::set<cPopulationCell*> occupied_cell_set;
			GetCell()->GetOccupiedNeighboringCells(occupied_cell_set, 1);
			if(occupied_cell_set.size()==0) {
				// nothing to do here, there are no neighbors
				return;
			}
			std::set<cPopulationCell*>::iterator selected=occupied_cell_set.begin();
			std::advance(selected, ctx.GetRandom().GetInt(occupied_cell_set.size()));
			source = *selected;
			break;
		}
		case 1: { // faced individual
			source = GetCellFaced();
			if(!source->IsOccupied()) {
				// nothing to do, we're facing an empty cell.
				return;
			}
			break;
		}
		default: {
			m_world->GetDriver().RaiseFatalException(1, "HGT_CONJUGATION_METHOD is set to an invalid value.");
			break;
		}
	}
	assert(source != 0);
	fragment_list_type fragments;
	cGenomeUtil::RandomSplit(ctx, 
													 m_world->GetConfig().HGT_FRAGMENT_SIZE_MEAN.Get(),
													 m_world->GetConfig().HGT_FRAGMENT_SIZE_VARIANCE.Get(),
													 source->GetOrganism()->GetGenome().GetSequence(),
													 fragments);
	ReceiveHGTDonation(fragments[ctx.GetRandom().GetInt(fragments.size())]);	
}


/*! Perform an HGT mutation on this offspring. 
 
 HGT mutations are location-dependent, hence they are piped through the population
 interface as opposed to being implemented in the CPU or organism.

 There is the possibility that more than one HGT mutation occurs when this method 
 is called.
 */
void cPopulationInterface::DoHGTMutation(cAvidaContext& ctx, Genome& offspring) {
	InitHGTSupport();
	
	// first, gather up all the fragments that we're going to be inserting into this offspring:
	// these come from a per-replication conjugational event:
	if((m_world->GetConfig().HGT_CONJUGATION_P.Get() > 0.0)
		 && (ctx.GetRandom().P(m_world->GetConfig().HGT_CONJUGATION_P.Get()))) {
		DoHGTConjugation(ctx);
	}	
	
	// the pending list includes both the fragments selected via the above process,
	// as well as from population-level conjugational events (see cPopulationActions.cc:cActionAvidianConjugation).
	fragment_list_type& fragments = m_hgt_support->_pending;

	// these come from "natural" competence (ie, eating the dead):
	if((m_world->GetConfig().HGT_COMPETENCE_P.Get() > 0.0)
		 && (ctx.GetRandom().P(m_world->GetConfig().HGT_COMPETENCE_P.Get()))) {
		
		// get this organism's cell:
		cPopulationCell& cell = m_world->GetPopulation().GetCell(m_cell_id);
		
		// the hgt source controls where the genetic material for HGT comes from.
		switch(m_world->GetConfig().HGT_SOURCE.Get()) {
			case 0: { // source is other genomes, nothing to do here (default)
				break;
			}
			case 1: { // source is the parent (a control)
				// this is a little hackish, but this is the cleanest way to make sure
				// that all downstream stuff works right.
				cell.ClearFragments(ctx);
				cell.AddGenomeFragments(ctx, cell.GetOrganism()->GetGenome().GetSequence());
				break;
			}
			default: { // error
				m_world->GetDriver().RaiseFatalException(1, "HGT_SOURCE is set to an invalid value.");
				break;
			}
		}
		
		// do we have any fragments available?
		if(cell.CountGenomeFragments() > 0) {
			// add a randomly-selected fragment to the list of fragments to be HGT'd,
			// remove it from the cell, and adjust the level of HGT resource.
			fragment_list_type::iterator selected=cell.GetFragments().begin();
			std::advance(selected, ctx.GetRandom().GetInt(cell.GetFragments().size()));
			fragments.insert(fragments.end(), *selected);			
			m_world->GetPopulation().AdjustHGTResource(ctx, -selected->GetSize());
			cell.GetFragments().erase(selected);
		}
	}
	
	// now, for each fragment being HGT'd, figure out where to put it:
	for(fragment_list_type::iterator i=fragments.begin(); i!=fragments.end(); ++i) {
		cGenomeUtil::substring_match location;
		switch(m_world->GetConfig().HGT_FRAGMENT_SELECTION.Get()) {
			case 0: { // match placement
				HGTMatchPlacement(ctx, offspring.GetSequence(), i, location);
				break;
			}
			case 1: { // match placement with redundant instruction trimming
				HGTTrimmedPlacement(ctx, offspring.GetSequence(), i, location);
				break;
			}
			case 2: { // random placement
				HGTRandomPlacement(ctx, offspring.GetSequence(), i, location);
				break;
			}
			default: { // error
				m_world->GetDriver().RaiseFatalException(1, "HGT_FRAGMENT_SELECTION is set to an invalid value.");
				break;
			}
		}
		
		// at this stage, we have a fragment and a location we're going to put it.
		// there are various transformations to this fragment that we could perform,
		// more as controls than anything else.
		switch(m_world->GetConfig().HGT_FRAGMENT_XFORM.Get()) {
			case 0: { // no transformation.
				break;
			}
			case 1: { // random shuffle of the instructions in the fragment.
				cGenomeUtil::RandomShuffle(ctx, *i);
				break;
			}
			case 2: { // replace the instructions in the fragment with random instructions.
				const cInstSet& instset = m_world->GetHardwareManager().GetInstSet(offspring.GetInstSet());
				for(int j=0; j<i->GetSize(); ++j) {
					(*i)[j] = instset.GetRandomInst(ctx);
				}
				break;
			}
			default: { // error
				m_world->GetDriver().RaiseFatalException(1, "HGT_FRAGMENT_XFORM is set to an invalid value.");
				break;
			}				
		}
		
		// do the mutation; we currently support insertions and replacements, but this can
		// be extended in the same way as fragment selection if need be.
		if(ctx.GetRandom().P(m_world->GetConfig().HGT_INSERTION_MUT_P.Get())) {
			// insert the fragment just after the final location:
			offspring.GetSequence().Insert(location.end, *i);
		} else {
			// replacement: replace [begin,end) instructions in the genome with the fragment,
			// respecting circularity.
			offspring.GetSequence().Replace(*i, location.begin, location.end);
		}
		
		// stats tracking:
		m_world->GetStats().GenomeFragmentInserted(GetOrganism(), *i, location);
	}
	
	// clean-up; be sure to empty the pending list so that we don't end up doing an HGT
	// operation multiple times on the same fragment.
	fragments.clear();
}


/*! Place the fragment at the location of best match.
 */
void cPopulationInterface::HGTMatchPlacement(cAvidaContext& ctx, const Sequence& offspring,
																						 fragment_list_type::iterator& selected,
																						 substring_match& location) {
	// find the location within the offspring's genome that best matches the selected fragment:
	location = cGenomeUtil::FindUnbiasedCircularMatch(ctx, offspring, *selected);
}


/*! Place the fragment at the location of best match, with redundant instructions trimmed.

 In this fragment selection method, the
 match location within the genome is calculated on a "trimmed" fragment.  Specifically,
 the trimmed fragment has all duplicate instructions at its end removed prior to the match.
 
 Mutations to the offspring are still performed using the entire fragment, so this effectively
 increases the insertion rate.  E.g., hgt(abcde, abcccc) -> abccccde.
 */
void cPopulationInterface::HGTTrimmedPlacement(cAvidaContext& ctx, const Sequence& offspring,
																											 fragment_list_type::iterator& selected,
																											 substring_match& location) {
	// copy the selected fragment, trimming redundant instructions at the end:
	Sequence trimmed(*selected);
	while((trimmed.GetSize() >= 2) && (trimmed[trimmed.GetSize()-1] == trimmed[trimmed.GetSize()-2])) {
		trimmed.Remove(trimmed.GetSize()-1);
	}
	
	// find the location within the offspring's genome that best matches the selected fragment:
	location = cGenomeUtil::FindUnbiasedCircularMatch(ctx, offspring, trimmed);
}


/*! Place the fragment at a random location.
 
 Here we select a random location for the fragment within the offspring.
 The beginning of the fragment location is selected at random, while the end is selected a
 random distance (up to the length of the selected fragment * 2) instructions away.
 */
void cPopulationInterface::HGTRandomPlacement(cAvidaContext& ctx, const Sequence& offspring,
																											fragment_list_type::iterator& selected,
																											substring_match& location) {
	// select a random location within the offspring's genome for this fragment to be
	// inserted:
	location.begin = ctx.GetRandom().GetUInt(offspring.GetSize());
	location.end = location.begin + ctx.GetRandom().GetUInt(selected->GetSize()*2);
	location.size = offspring.GetSize();
	location.resize(offspring.GetSize());
}

/*! Called when this organism is the receiver of an HGT donation.
 */
void cPopulationInterface::ReceiveHGTDonation(const Sequence& fragment) {
	InitHGTSupport();
	m_hgt_support->_pending.push_back(fragment);
}


bool cPopulationInterface::Move(cAvidaContext& ctx, int src_id, int dest_id)
{
  return m_world->GetPopulation().MoveOrganisms(ctx, src_id, dest_id, -1);
}

bool cPopulationInterface::MoveAvatar(cAvidaContext& ctx, int src_id, int dest_id, int true_cell)
{
  bool success = m_world->GetPopulation().MoveOrganisms(ctx, src_id, dest_id, true_cell);
  if (success) { 
    assert (m_world->GetPopulation().GetCell(src_id).HasAvatar());
    m_world->GetPopulation().GetCell(src_id).RemoveAvatar(GetOrganism());
    m_world->GetPopulation().GetCell(dest_id).AddAvatar(GetOrganism());
  }
  return success;
}

// ALWAYS set cell first, facing second, faced cell third.
// record avatar cell location any time avatar is moved, injected, or born into cell (not on rotate)
void cPopulationInterface::SetAVCellID(int av_cell_id) 
{ 
  m_av_cell_id = av_cell_id; 
}

// needs to be called on inject, birth, and rotate (not on move)
void cPopulationInterface::SetAvatarFacing(int facing)
{
  m_av_facing = facing;
}

// record avatar faced cell any time avatar is moved, injected, born into cell, or rotates
void cPopulationInterface::SetAvatarFacedCell(int av_cell_id) 
{ 
  // rotate avatar cell to correct direction for this avatar, then get faced cell
  cPopulationCell & av_cell = m_world->GetPopulation().GetCell(m_av_cell_id);
  for (int i = 0; i < av_cell.ConnectionList().GetSize(); i++) {
    av_cell.ConnectionList().CircNext();
    if (av_cell.GetFacedDir() == m_av_facing) break;
  }
  m_av_cell_faced = m_world->GetPopulation().GetCell(m_av_cell_id).GetCellFaced().GetID();    
}

void cPopulationInterface::AddLiveOrg()  
{
  m_world->GetPopulation().AddLiveOrg(GetOrganism());
}

void cPopulationInterface::RemoveLiveOrg() 
{
  m_world->GetPopulation().RemoveLiveOrg(GetOrganism());
}

bool cPopulationInterface::HasOpinion(cOrganism* in_organism)
{
  return in_organism->HasOpinion();
}

void cPopulationInterface::SetOpinion(int opinion, cOrganism* in_organism)
{
  in_organism->SetOpinion(opinion);
}

void cPopulationInterface::ClearOpinion(cOrganism* in_organism)
{
  in_organism->ClearOpinion();
}

void cPopulationInterface::JoinGroup(int group_id)
{
  m_world->GetPopulation().JoinGroup(GetOrganism(), group_id);
}

void cPopulationInterface::MakeGroup()
{
  m_world->GetPopulation().MakeGroup(GetOrganism());
}

void cPopulationInterface::LeaveGroup(int group_id)
{
  m_world->GetPopulation().LeaveGroup(GetOrganism(), group_id);
}

int cPopulationInterface::NumberOfOrganismsInGroup(int group_id)
{
  return m_world->GetPopulation().NumberOfOrganismsInGroup(group_id);
}

/* Increases tolerance towards the addition of members to the group.
 * toleranceType:
 *    0: increases tolerance towards immigrants
 *    1: increases tolerance towards own offspring
 *    2: increases tolerance towards other offspring of the group
 * Removes the most recent record of dec-tolerance
 * Returns the modified tolerance total.
 */
int cPopulationInterface::IncTolerance(const int toleranceType, cAvidaContext &ctx)
{
  int group_id = GetOrganism()->GetOpinion().first;
  
  if (toleranceType == 0) {
    // Modify tolerance towards immigrants
    PushToleranceInstExe(0, ctx);
    
    // Update tolerance list by removing the most recent dec_tolerance record
    delete GetOrganism()->GetPhenotype().GetToleranceImmigrants().Pop();
    
    // If not at individual's max tolerance, adjust both caches
    if (GetOrganism()->GetPhenotype().GetIntolerances()[0].second != 0) {
      GetOrganism()->GetPhenotype().GetIntolerances()[0].second--;
      GetGroupIntolerances(group_id, 0)--;
    }
    // Retrieve modified tolerance total for immigrants
    return GetOrganism()->GetPhenotype().CalcToleranceImmigrants();
  }
  if (toleranceType == 1) {
    // Modify tolerance towards own offspring
    PushToleranceInstExe(1, ctx);
    
    // Update tolerance list by removing the most recent dec_tolerance record
    delete  GetOrganism()->GetPhenotype().GetToleranceOffspringOwn().Pop();
    
    // If not at max tolerance, increase the cache
    if (GetOrganism()->GetPhenotype().GetIntolerances()[1].second != 0) {
      GetOrganism()->GetPhenotype().GetIntolerances()[1].second--;
    }
    // Retrieve modified tolerance total for own offspring.
    return GetOrganism()->GetPhenotype().CalcToleranceOffspringOwn();
  }
  if (toleranceType == 2) {
    // Modify tolerance towards other offspring of the group
    PushToleranceInstExe(2, ctx);
    
    // Update tolerance list by removing the most recent dec_tolerance record
    delete GetOrganism()->GetPhenotype().GetToleranceOffspringOthers().Pop();
    
    
    // If not at max tolerance, increase the cache
    if (GetOrganism()->GetPhenotype().GetIntolerances()[2].second != 0) {
      GetOrganism()->GetPhenotype().GetIntolerances()[2].second--;
      GetGroupIntolerances(group_id, 1)--;
    }
    // Retrieve modified tolerance total for other offspring in group.
    return GetOrganism()->GetPhenotype().CalcToleranceOffspringOthers();
  }
  return -1;
}

/* Decreases tolerance towards the addition of members to the group.
 * toleranceType:
 *    0: decreases tolerance towards immigrants
 *    1: decreases tolerance towards own offspring
 *    2: decreases tolerance towards other offspring of the group
 * Records the update during which dec-tolerance was executed
 * Returns the modified tolerance total.
 */
int cPopulationInterface::DecTolerance(const int toleranceType, cAvidaContext &ctx)
{
  const int cur_update = m_world->GetStats().GetUpdate();
  const int tolerance_max = m_world->GetConfig().MAX_TOLERANCE.Get();
  int group_id = GetOrganism()->GetOpinion().first;
  
  if (toleranceType == 0) {
    // Modify tolerance towards immigrants
    PushToleranceInstExe(3, ctx);
    
    // Update tolerance list by inserting new record (at the front)
    tList<int>& toleranceList = GetOrganism()->GetPhenotype().GetToleranceImmigrants();
    toleranceList.Push(new int(cur_update));
    if(toleranceList.GetSize() > tolerance_max) delete toleranceList.PopRear();
    
    // If not at min tolerance, decrease the cache
    if (GetOrganism()->GetPhenotype().GetIntolerances()[0].second != tolerance_max) {
      GetOrganism()->GetPhenotype().GetIntolerances()[0].second++;
      GetGroupIntolerances(group_id, 0)++;
    }
    
    // Return modified tolerance total for immigrants.
    return GetOrganism()->GetPhenotype().CalcToleranceImmigrants();
  }
  if (toleranceType == 1) {
    PushToleranceInstExe(4, ctx);
    
    // Update tolerance list by inserting new record (at the front)
    tList<int> &toleranceList = GetOrganism()->GetPhenotype().GetToleranceOffspringOwn();
    toleranceList.Push(new int(cur_update));
    if(toleranceList.GetSize() > tolerance_max) delete toleranceList.PopRear();
    
    // If not at min tolerance, decrease the cache
    if (GetOrganism()->GetPhenotype().GetIntolerances()[1].second != tolerance_max) {
      GetOrganism()->GetPhenotype().GetIntolerances()[1].second++;
    }
    // Return modified tolerance total for own offspring.
    return GetOrganism()->GetPhenotype().CalcToleranceOffspringOwn();

  }
  if (toleranceType == 2) {
    PushToleranceInstExe(5, ctx);
    
    // Update tolerance list by inserting new record (at the front)
    tList<int>& toleranceList = GetOrganism()->GetPhenotype().GetToleranceOffspringOthers();
    toleranceList.Push(new int(cur_update));
    if(toleranceList.GetSize() > tolerance_max) delete toleranceList.PopRear();
    
    // If not at min tolerance, decrease the cache
    if (GetOrganism()->GetPhenotype().GetIntolerances()[2].second != tolerance_max) {
      GetOrganism()->GetPhenotype().GetIntolerances()[2].second++;
      GetOrganism()->GetOrgInterface().GetGroupIntolerances(group_id, 1)++;
    }
    // Retrieve modified tolerance total for other offspring in the group.
    return GetOrganism()->GetPhenotype().CalcToleranceOffspringOthers();
  }
  
  return -1;
}

int cPopulationInterface::CalcGroupToleranceImmigrants(int prop_group_id)
{
  return m_world->GetPopulation().CalcGroupToleranceImmigrants(prop_group_id);
}

int cPopulationInterface::CalcGroupToleranceOffspring(cOrganism* parent_organism)
{
  return m_world->GetPopulation().CalcGroupToleranceOffspring(parent_organism);
}

double cPopulationInterface::CalcGroupOddsImmigrants(int group_id)
{
  return m_world->GetPopulation().CalcGroupOddsImmigrants(group_id);
}

double cPopulationInterface::CalcGroupOddsOffspring(cOrganism* parent)
{
  return m_world->GetPopulation().CalcGroupOddsOffspring(parent);
}

double cPopulationInterface::CalcGroupOddsOffspring(int group_id)
{
  return m_world->GetPopulation().CalcGroupOddsOffspring(group_id);
}

bool cPopulationInterface::AttemptImmigrateGroup(int group_id, cOrganism* org)
{
  return m_world->GetPopulation().AttemptImmigrateGroup(group_id, org);
}

void cPopulationInterface::PushToleranceInstExe(int tol_inst, cAvidaContext& ctx)
{
  if(!m_world->GetConfig().TRACK_TOLERANCE.Get()) {
    m_world->GetStats().PushToleranceInstExe(tol_inst);
    return;
  }
  
  const tArray<double> res_count = GetResources(ctx);
  
  int group_id = GetOrganism()->GetOpinion().first;
  int group_size = NumberOfOrganismsInGroup(group_id);
  double resource_level = res_count[group_id];
  int tol_max = m_world->GetConfig().MAX_TOLERANCE.Get();
  
  double immigrant_odds = CalcGroupOddsImmigrants(group_id);
  double offspring_own_odds;
  double offspring_others_odds;
  int tol_immi = GetOrganism()->GetPhenotype().CalcToleranceImmigrants();
  int tol_own;
  int tol_others;
  
  if(m_world->GetConfig().TOLERANCE_VARIATIONS.Get() == 1) {
    offspring_own_odds = 1.0;
    offspring_others_odds = 1.0;
    tol_own = tol_max;
    tol_others = tol_max;
  } else {
    offspring_own_odds = CalcGroupOddsOffspring(GetOrganism());
    offspring_others_odds = CalcGroupOddsOffspring(group_id);
    tol_own = GetOrganism()->GetPhenotype().CalcToleranceOffspringOwn();
    tol_others = GetOrganism()->GetPhenotype().CalcToleranceOffspringOthers();
  }
  
  double odds_immi = immigrant_odds * 100;
  double odds_own = offspring_own_odds * 100;
  double odds_others = offspring_others_odds * 100;
  
  m_world->GetStats().PushToleranceInstExe(tol_inst, group_id, group_size, resource_level, odds_immi, odds_own, odds_others, tol_immi, tol_own, tol_others, tol_max);
  return;
}

int& cPopulationInterface::GetGroupIntolerances(int group_id, int tol_num)
{
  return m_world->GetPopulation().GetGroupIntolerances(group_id, tol_num);
}

void cPopulationInterface::AttackFacedOrg(cAvidaContext& ctx, int loser)
{
  m_world->GetPopulation().AttackFacedOrg(ctx, loser);
}

void cPopulationInterface::BeginSleep()
{
  if(m_world->GetConfig().LOG_SLEEP_TIMES.Get() == 1)
    m_world->GetPopulation().AddBeginSleep(m_cell_id, m_world->GetStats().GetUpdate());
  GetDeme()->IncSleepingCount();
}

void cPopulationInterface::EndSleep()
{
  if(m_world->GetConfig().LOG_SLEEP_TIMES.Get() == 1)
    m_world->GetPopulation().AddEndSleep(m_cell_id, m_world->GetStats().GetUpdate());
  GetDeme()->DecSleepingCount();
}
