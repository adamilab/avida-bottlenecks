/*
 *  cTestCPUOrgInterface.h
 *  Avida
 *
 *  Created by David on 3/4/06.
 *  Copyright 1999-2011 Michigan State University. All rights reserved.
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

#ifndef cTestCPUInterface_h
#define cTestCPUInterface_h

#ifndef cOrgInterface_h
#include "cOrgInterface.h"
#endif

class cCPUTestInfo;
class cTestCPU;

#ifndef NULL
#define NULL 0
#endif

class cTestCPUInterface : public cOrgInterface
{
private:
  cTestCPU* m_testcpu;
  cCPUTestInfo& m_test_info;
  int m_cur_depth;

public:
  cTestCPUInterface(cTestCPU* testcpu, cCPUTestInfo& test_info, int cur_depth)
    : m_testcpu(testcpu), m_test_info(test_info), m_cur_depth(cur_depth) { ; }
  virtual ~cTestCPUInterface() { ; }

  
  tSmartArray <cOrganism*> GetLiveOrgList();
  cPopulationCell* GetCell() { return NULL; }
	cPopulationCell* GetCell(int cell_id) { return NULL; }
  int GetCellID() { return -1; }
  int GetDemeID() { return -1; }
  cDeme* GetDeme() { return 0; }
  void SetCellID(int in_id) { ; }
  void SetDemeID(int in_id) { ; }
  
  int GetAVCellID() { return -1; }
  void SetAVCellID(int av_cell_id) { ; }
  void SetAvatarFacing(int facing) { ; }
  void SetAvatarFacedCell(int av_cell_id) { ; }
  int GetAVFacedCellID() { return -1; }
  int GetAVFacedDir() { return 0; }
  
  int GetCellData() { return -1; }
  int GetCellDataOrgID() { return -1; }
  int GetCellDataUpdate() { return -1; }
  int GetCellDataTerritory() { return -1; }
  int GetCellDataForagerType() { return -99; }
  void SetCellData(const int newData) { ; }
  void SetAVCellData(const int newData, const int org_id) { ; }
  int GetFacedCellData() { return -1; }
  int GetFacedCellDataOrgID() { return -1; }
  int GetFacedCellDataUpdate() { return -1; }
  int GetFacedCellDataTerritory() { return -1; }
  int GetFacedAVData() { return -1; }
  int GetFacedAVDataOrgID() { return -1; }
  int GetFacedAVDataUpdate() { return -1; }
  int GetFacedAVDataTerritory() { return -1; }

  int GetPrevSeenCellID() { return 0; }
  int GetPrevTaskCellID() { return 0; }
  void AddReachedTaskCell() { }
  int GetNumTaskCellsReached() { return 0; }
  void SetPrevSeenCellID(int in_id) { ; }
  void SetPrevTaskCellID(int in_id) { ; }

  bool Divide(cAvidaContext& ctx, cOrganism* parent, const Genome& offspring_genome);
  cOrganism* GetNeighbor();
  cOrganism* GetAVRandNeighbor();
  cOrganism* GetAVRandNeighborPrey();
  cOrganism* GetAVRandNeighborPred();
  tArray<cOrganism*> GetAVNeighbors();
  tArray<cOrganism*> GetAVNeighborPrey();
  bool IsNeighborCellOccupied();
  bool HasAVNeighbor();
  bool HasAVNeighborPrey();
  bool HasAVNeighborPred();
  int GetNumNeighbors();
  int GetAVNumNeighbors();
  void GetNeighborhoodCellIDs(tArray<int>& list);
  int GetNeighborCellContents() { return 0; }
  void Rotate(int direction = 1);
  void Breakpoint() { ; }
  int GetInputAt(int& input_pointer);
  void ResetInputs(cAvidaContext& ctx);
  const tArray<int>& GetInputs() const;
  const tArray<double>& GetResources(cAvidaContext& ctx); 
  const tArray<double>& GetAVResources(cAvidaContext& ctx); 
  const tArray<double>& GetFacedCellResources(cAvidaContext& ctx); 
  const tArray<double>& GetFacedAVResources(cAvidaContext& ctx); 
  const tArray<double>& GetDemeResources(int deme_id, cAvidaContext& ctx); 
  const tArray<double>& GetCellResources(int cell_id, cAvidaContext& ctx); 
  const tArray<double>& GetFrozenResources(cAvidaContext& ctx, int cell_id);
  const tArray< tArray<int> >& GetCellIdLists();  
  int GetCurrPeakX(cAvidaContext& ctx, int res_id) { return 0; } 
  int GetCurrPeakY(cAvidaContext& ctx, int res_id) { return 0; } 
  int GetFrozenPeakX(cAvidaContext& ctx, int res_id) { return 0; } 
  int GetFrozenPeakY(cAvidaContext& ctx, int res_id) { return 0; } 
  void TriggerDoUpdates(cAvidaContext& ctx) { }
  void UpdateResources(cAvidaContext& ctx, const tArray<double>& res_change);
  void UpdateAVResources(cAvidaContext& ctx, const tArray<double>& res_change);
  void UpdateDemeResources(cAvidaContext& ctx, const tArray<double>& res_change) {;}
  void Die(cAvidaContext& ctx); 
  void KillCellID(int target, cAvidaContext& ctx); 
  void Kaboom(int distance, cAvidaContext& ctx); 
  void SpawnDeme(cAvidaContext& ctx); 
  cOrgSinkMessage* NetReceive() { return NULL; } // @DMB - todo: receive message
  bool NetRemoteValidate(cAvidaContext& ctx, cOrgSinkMessage* msg) { return false; } // @DMB - todo: validate message
  int ReceiveValue();
  void SellValue(const int data, const int label, const int sell_price, const int org_id);
  int BuyValue(const int label, const int buy_price);
  bool InjectParasite(cOrganism* host, cBioUnit* parent, const cString& label, const Sequence& injected_code);
  bool UpdateMerit(double new_merit);
  bool TestOnDivide() { return false; }
  int GetFacing() { return 0; }
  int GetFacedCellID() { return -1; }
  int GetFacedDir() { return 0; }
  bool SendMessage(cOrgMessage& msg) { return false; }
  bool SendMessage(cOrganism* recvr, cOrgMessage& msg) { return false; }
	bool BroadcastMessage(cOrgMessage& msg, int depth) { return false; }
	bool BcastAlarm(int jump_label, int bcast_range) { return false; }
  void DivideOrgTestamentAmongDeme(double value) {;}
	void SendFlash() { }
  
  int GetNortherly() {return 0; }
  int GetEasterly() {return 0; }
	
	void RotateToGreatestReputation(){ }
	void RotateToGreatestReputationWithDifferentTag(int tag){ }
	void RotateToGreatestReputationWithDifferentLineage(int tag){ }	
  
  int GetStateGridID(cAvidaContext& ctx);
	
	//! Link this organism's cell to the cell it is currently facing.
	void CreateLinkByFacing(double weight=1.0) { }
	//! Link this organism's cell to the cell with coordinates (x,y).
	void CreateLinkByXY(int x, int y, double weight=1.0) { }
	//! Link this organism's cell to the cell with index idx.
	void CreateLinkByIndex(int idx, double weight=1.0) { }
	//! Broadcast a message to all organisms that are connected by this network.
	bool NetworkBroadcast(cOrgMessage& msg) { return false; }
	//! Unicast a message to the current selected organism.
	bool NetworkUnicast(cOrgMessage& msg) { return false; }
	//! Rotate to select a new network link.
	bool NetworkRotate(int x) { return false; }
	//! Select a new network link.
	bool NetworkSelect(int x) { return false; }	

	//! HGT donation (does nothing).
	void DoHGTDonation(cAvidaContext& ctx) { }
	//! HGT conjugation (does nothing).
	void DoHGTConjugation(cAvidaContext& ctx) { }
	//! HGT mutation (does nothing).
	void DoHGTMutation(cAvidaContext& ctx, Genome& offspring) { }
	//! Receive HGT donation (does nothing).
	void ReceiveHGTDonation(const Sequence& fragment) { }
  
  bool Move(cAvidaContext& ctx, int src_id, int dest_id) { return false; }
  bool MoveAvatar(cAvidaContext& ctx, int src_id, int dest_id, int true_cell) { return false; }

  void AddLiveOrg() { ; }  
  void RemoveLiveOrg() { ; }  
  
  bool HasOpinion(cOrganism* in_organism) { return false; }
  void SetOpinion(int opinion, cOrganism* in_organism) { ; }
  void ClearOpinion(cOrganism* in_organism) { ; }

  void JoinGroup(int group_id) { ; }
  void MakeGroup() { ; }
  void LeaveGroup(int group_id) { ; }
  int NumberOfOrganismsInGroup(int group_id) {return 0; }
    
  int IncTolerance(int toleranceType, cAvidaContext &ctx) {return 0;}
  int DecTolerance(int toleranceType, cAvidaContext &ctx) {return 0;}
  int CalcGroupToleranceImmigrants(int prop_group_id) {return 0; }
  int CalcGroupToleranceOffspring(cOrganism* parent_organism) {return 0; }
  double CalcGroupOddsImmigrants(int group_id) {return 0.0; }
  double CalcGroupOddsOffspring(cOrganism* parent) {return 0.0; }
  double CalcGroupOddsOffspring(int group_id) {return 0.0; }
  bool AttemptImmigrateGroup(int group_id, cOrganism* org) {return false; }
  void PushToleranceInstExe(int tol_inst, cAvidaContext &ctx) { ; }
  int& GetGroupIntolerances(int group_id, int tol_num) { return *(new int(0)); }

  void AttackFacedOrg(cAvidaContext& ctx, int loser) { ; }
  
  void BeginSleep() { ; }
  void EndSleep() { ; }
};

#endif
