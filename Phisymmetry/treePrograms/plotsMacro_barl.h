//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Dec  2 16:57:52 2011 by ROOT version 5.27/06b
// from TChain variablesTree_barl/
//////////////////////////////////////////////////////////

#ifndef plotsMacro_barl_h
#define plotsMacro_barl_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#define MAXHITS 1000

class plotsMacro_barl {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t          nhit;
   Int_t           ieta[MAXHITS];   //[nhit]
   Int_t           iphi[MAXHITS];   //[nhit]
   Float_t         lc_barl[MAXHITS];   //[nhit]
   Float_t         et_barl[MAXHITS];   //[nhit]
   Int_t          unixtime;
   Int_t          run;
   Int_t          lumi;
   Int_t          eventid;

   TBranch        *b_nhit;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_lc_barl;   //!
   TBranch        *b_et_barl;   //!
   TBranch        *b_unixtime;   //!
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_eventid;   //!




   plotsMacro_barl(TTree *tree=0);
   virtual ~plotsMacro_barl();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef plotsMacro_barl_cxx
plotsMacro_barl::plotsMacro_barl(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f) {
         f = new TFile("Memory Directory");
         f->cd("Rint:/");
      }
      tree = (TTree*)gDirectory->Get("variablesTree_barl");

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("variablesTree_barl","");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_100_1_Ipg.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_101_1_D0Z.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_102_1_Vi1.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_103_1_Jok.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_104_1_C1g.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_105_1_2lc.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_106_1_7Yh.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_107_1_9SJ.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_108_1_c51.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_109_1_b43.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_10_1_LCw.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_110_1_CbD.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_111_1_zKZ.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_112_1_1FZ.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_113_1_2vX.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_114_1_HDx.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_115_1_FYo.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_116_1_Exm.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_117_1_MaM.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_118_1_57D.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_119_1_8te.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_11_1_q51.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_120_1_Vi0.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_121_1_va7.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_122_1_4PT.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_123_1_vnL.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_124_1_5dS.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_125_1_Gzc.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_126_1_MOD.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_127_1_5Vk.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_128_1_QpM.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_129_1_SdS.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_12_1_QW6.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_130_1_nuc.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_131_1_A4U.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_132_1_jeQ.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_133_1_KbH.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_134_1_bVz.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_135_1_Igo.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_136_1_5SU.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_137_1_P9k.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_138_1_Y70.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_13_1_K1p.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_14_1_jAm.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_15_1_VqA.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_16_1_6lK.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_17_1_PqJ.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_18_1_EH9.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_19_1_v8X.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_1_1_cIr.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_20_1_f7c.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_21_1_e3g.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_22_1_fRj.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_23_1_Wk2.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_24_1_0Mu.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_25_1_zA7.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_26_1_CK0.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_27_1_E6W.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_28_1_QxI.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_29_1_vTk.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_2_1_Nxw.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_30_1_aWO.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_31_1_Hkp.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_32_1_8Ht.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_33_1_ANR.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_34_1_tSJ.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_35_1_4GI.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_36_1_3Y1.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_37_1_hiv.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_38_1_SPs.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_39_1_IvZ.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_3_1_bMx.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_40_1_Ipv.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_41_1_vn9.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_42_1_jJS.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_43_1_K2c.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_44_1_PNO.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_45_1_wDe.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_46_1_cBP.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_47_1_JfW.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_48_1_hLe.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_49_1_lvd.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_4_1_Avy.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_50_1_KYu.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_51_1_xu9.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_52_1_x6M.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_53_1_upD.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_54_1_1fj.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_55_1_C7J.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_56_1_bQ2.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_57_1_llE.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_58_1_0YT.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_59_1_w9Z.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_5_1_gvk.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_60_1_NdQ.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_61_1_sis.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_62_1_Hj5.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_63_1_SBm.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_64_1_dpt.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_65_1_2fi.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_66_1_uE6.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_67_1_WHq.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_68_1_d1S.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_69_1_dY9.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_6_1_lMo.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_70_1_Gin.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_71_1_cLy.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_72_1_ol7.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_73_1_HE2.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_74_1_TY9.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_75_1_9ML.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_76_1_Bj0.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_77_1_6as.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_78_1_UB7.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_79_1_Cud.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_7_1_yoa.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_80_1_vLe.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_81_1_RA3.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_82_1_j8Y.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_83_1_PYQ.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_84_1_fUb.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_85_1_qJv.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_86_1_NFr.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_87_1_EJi.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_88_1_B5i.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_89_1_Zh5.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_8_1_t77.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_90_1_Ob6.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_91_1_5uS.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_92_1_4um.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_93_1_iwX.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_94_1_eCd.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_95_1_qgQ.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_96_1_7gc.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_97_1_IeN.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_98_1_21z.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_99_1_RRe.root/variablesTree_barl");
      chain->Add("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees/172947_174137/variablesTree_9_1_Bha.root/variablesTree_barl");
      chain->Add("/variablesTree_barl");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

plotsMacro_barl::~plotsMacro_barl()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t plotsMacro_barl::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t plotsMacro_barl::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void plotsMacro_barl::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);
   fChain->SetBranchAddress("nhit", &nhit, &b_nhit);
   fChain->SetBranchAddress("ieta", ieta, &b_ieta);
   fChain->SetBranchAddress("iphi", iphi, &b_iphi);
   fChain->SetBranchAddress("lc_barl", lc_barl, &b_lc_barl);
   fChain->SetBranchAddress("et_barl", et_barl, &b_et_barl);
   fChain->SetBranchAddress("unixtime", &unixtime, &b_unixtime);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("eventid", &eventid, &b_eventid);
   

   
   Notify();
}

Bool_t plotsMacro_barl::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void plotsMacro_barl::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t plotsMacro_barl::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef plotsMacro_barl_cxx
