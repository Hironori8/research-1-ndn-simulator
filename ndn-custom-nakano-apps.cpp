#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <time.h>
#include <cstdio>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/socket.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"

#define TOTAL_NODE 13
#define ROUTER1_NUM 10
#define ROUTER2_NUM 11
#define SERVER_NUM 12


// define -----
double MIN_TIME = 1.0;
double SENDING_RATE = 40.0; 
double SIMULATION_TIME = 60.0;

namespace ns3 {


int
main(int argc, char* argv[])
{
  // time_t now = std::time(nullptr);
  // char Anim[60];
  // int n1 = sprintf(Anim,"./Result/Anim/AnimFile_%s.csv",ctime(&now));
  // std::string animFile = Anim;
  
  CommandLine cmd;
  cmd.Parse(argc, argv);

  
  NodeContainer nodes;
  nodes.Create(TOTAL_NODE);
  
  
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  for(int i = 0; i < TOTAL_NODE; i++){
    if((i!= ROUTER1_NUM)&&(i!=ROUTER2_NUM)&&(i!=SERVER_NUM))
    {
      p2p.Install(nodes.Get(i),nodes.Get(ROUTER1_NUM));
    }
  }
  p2p.Install(nodes.Get(ROUTER1_NUM), nodes.Get(ROUTER2_NUM));
  p2p.Install(nodes.Get(ROUTER2_NUM), nodes.Get(SERVER_NUM));
  
  
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Nocache");
  NodeContainer UserAndServerNodes;
  for(NodeList::Iterator i=NodeList::Begin();i!=NodeList::End();++i){
    if(*i!=nodes.Get(ROUTER1_NUM)){
      if(*i!=nodes.Get(ROUTER2_NUM)){
      UserAndServerNodes.Add(*i);
      }
    }
  }
  
  ndnHelper.Install(UserAndServerNodes);
  
  
  ndn::StackHelper ndnHelper2;
  ndnHelper2.setCsSize(1000);
  ndnHelper2.setPolicy("nfd::cs::slru");
  ndnHelper2.Install(nodes.Get(ROUTER1_NUM));
  
  ndn::StackHelper ndnHelper3;
  ndnHelper3.setCsSize(1000);
  ndnHelper3.setPolicy("nfd::cs::slruNoPrint");
  ndnHelper3.Install(nodes.Get(ROUTER2_NUM));
  
  
  ndn::StrategyChoiceHelper::InstallAll
    ("/", "/localhost/nfd/strategy/best-route");
  
  
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();
   
  
  ndn::AppHelper user1("CustomConsumer");
  
  user1.SetPrefix("/prefix");
  
  user1.SetAttribute("Frequency",StringValue("300"));
  
  NodeContainer UserNodes_Verification;
  for(NodeList::Iterator i=NodeList::Begin();i!=NodeList::End();++i){
    if(*i!=nodes.Get(ROUTER1_NUM)&&*i!=nodes.Get(ROUTER2_NUM)
        &&*i!=nodes.Get(SERVER_NUM)){
      UserNodes_Verification.Add(*i);
    }
  }
  user1.Install(UserNodes_Verification);
  
  
  // ndn::AppHelper user2("CustomConsumer1");
  // user2.SetPrefix("/prefix/sub");
  // // user2.SetAttribute("Frequency"
  // // ,StringValue( std::to_string( SENDING_RATE ) ));
  // user2.SetAttribute("Frequency",StringValue("2"));
  // user2.Install(nodes.Get(0));
 
  
  // ndn::AppHelper user3("CustomConsumer2");
  
  // user3.SetPrefix("/prefix");
  
  //user3.SetAttribute("Frequency"
  //,StringValue( std::to_string( SENDING_RATE ) ));
  // user3.SetAttribute("Frequency",StringValue("300"));
  
  // NodeContainer UserNodes;
  // for(NodeList::Iterator i=NodeList::Begin();i!=NodeList::End();++i){
    // if(*i!=nodes.Get(ROUTER1_NUM)&&*i!=nodes.Get(ROUTER2_NUM)
        // &&*i!=nodes.Get(SERVER_NUM)
        // &&*i!=nodes.Get(ATTACKER1_NUM)&&*i!=nodes.Get(ATTACKER2_NUM)){
      // UserNodes.Add(*i);
    // }
  // }
  // user3.Install(UserNodes);
  
  // NodeContainer UserNodes;
  // int j1 = 0;
  // for(NodeList::Iterator i=NodeList::Begin();i!=NodeList::End();++i){
    // if(*i!=nodes.Get(ROUTER1_NUM)&&*i!=nodes.Get(ROUTER2_NUM)
        // &&*i!=nodes.Get(SERVER_NUM)){
      // if(ATTACK_TOTAL<=j1){
        // UserNodes.Add(*i);
      // }
    // }
    // j1++;
  // }
  // user3.Install(UserNodes);
  
  ndn::AppHelper router1("CustomRouter2");
  
  router1.Install(nodes.Get(ROUTER1_NUM));
  

  
  ndn::AppHelper router2("CustomRouter2");
  
  router2.Install(nodes.Get(ROUTER2_NUM));
  
  ndn::AppHelper server("CustomProducer");
  
  // serverHelper.SetAttribute("PayloadSize",StringValue("1024"));
  server.Install(nodes.Get(SERVER_NUM)); 
  
  ndnGlobalRoutingHelper.AddOrigins("/prefix",nodes.Get(SERVER_NUM));
  ndnGlobalRoutingHelper.AddOrigins("/prefix/sub",nodes.Get(SERVER_NUM));
  ndnGlobalRoutingHelper.AddOrigins("/verification/0",nodes.Get(SERVER_NUM));
  ndnGlobalRoutingHelper.AddOrigins("/verification/1",nodes.Get(SERVER_NUM));
  ndnGlobalRoutingHelper.AddOrigins("/verification/2",nodes.Get(SERVER_NUM));
  ndnGlobalRoutingHelper.AddOrigins("/verification/3",nodes.Get(SERVER_NUM));
  ndnGlobalRoutingHelper.AddOrigins("/verification/4",nodes.Get(SERVER_NUM));
  ndnGlobalRoutingHelper.AddOrigins("/verification/5",nodes.Get(SERVER_NUM));
  ndnGlobalRoutingHelper.AddOrigins("/verification/6",nodes.Get(SERVER_NUM));
  ndnGlobalRoutingHelper.AddOrigins("/verification/7",nodes.Get(SERVER_NUM));
  ndnGlobalRoutingHelper.AddOrigins("/verification/8",nodes.Get(SERVER_NUM));
  ndnGlobalRoutingHelper.AddOrigins("/verification/9",nodes.Get(SERVER_NUM));
  ndn::GlobalRoutingHelper::CalculateRoutes();
  
 
  Simulator::Stop(Seconds(SIMULATION_TIME));
  // Simulator::Stop(Seconds(40.0 ));
  
  
  // AnimationInterface anim (animFile);
  
  // anim.EnablePacketMetadata ();
  
  // anim.SetConstantPosition(nodes.Get(0),0,78);
  // anim.SetConstantPosition(nodes.Get(1),35,35);
  // anim.SetConstantPosition(nodes.Get(2),35,0);
  // anim.SetConstantPosition(nodes.Get(3),71,78);
  
  
  Simulator::Run();
  
  
  // std::cout << "Animation Trace file created:" 
    // << animFile.c_str()<< std::endl;
  
  
  Simulator::Destroy();

  return 0;
}


}

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
