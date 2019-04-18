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
//$B%N!<%I$NAm?t(B
#define ROUTER1_NUM 10
//$B%k!<%?#1$N%N!<%I(BID
#define ROUTER2_NUM 11
//r$B%k!<%?#2$N%N!<%I(BID
#define SERVER_NUM 12
//$B%5!<%P$N%N!<%I(BID

// define -----
double MIN_TIME = 1.0;
double SENDING_RATE = 40.0; 
double SIMULATION_TIME = 60.0;

//ns3$B$NL>A06u4V$N@_Dj(B
namespace ns3 {


int
main(int argc, char* argv[])
{
  // time_t now = std::time(nullptr);
  // char Anim[60];
  // int n1 = sprintf(Anim,"./Result/Anim/AnimFile_%s.csv",ctime(&now));
  //$B%"%K%a!<%7%g%s%U%!%$%k$NL>A0(B
  // std::string animFile = Anim;
  
  CommandLine cmd;
  cmd.Parse(argc, argv);

  //$B%N!<%I$N:n@.(B
  NodeContainer nodes;
  nodes.Create(TOTAL_NODE);
  
  //p2p $B%N!<%I$r7R$2$k(B
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
  
  //$B%5!<%P!<$H%f!<%6$K(Bndn$B$H$7$FI,MW$J5!G=$rDI2C(B
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
  
  //$B%k!<%?$K(Bndn$B$H$7$FI,MW$J5!G=$rDI2C(B 
  ndn::StackHelper ndnHelper2;
  ndnHelper2.setCsSize(1000);
  ndnHelper2.setPolicy("nfd::cs::slru");
  ndnHelper2.Install(nodes.Get(ROUTER1_NUM));
  
  ndn::StackHelper ndnHelper3;
  ndnHelper3.setCsSize(1000);
  ndnHelper3.setPolicy("nfd::cs::slruNoPrint");
  ndnHelper3.Install(nodes.Get(ROUTER2_NUM));
  
  //$BMW5a%Q%1%C%H$,$I$N$h$&$K%k!<%H$rC5:w$9$k$+$r@_Dj(B
  ndn::StrategyChoiceHelper::InstallAll
    ("/", "/localhost/nfd/strategy/best-route");
  
  //$BA4$F$N%N!<%I$K(Bndn$B$N%k!<%F%#%s%0$N5!G=$rDI2C(B
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();
   
  //$B%f!<%6967b$"$j(B 
  ndn::AppHelper user1("CustomConsumer");
  //$B%f!<%6$N5sF0$r=q$$$?%*%V%8%'%/%H$r:n@.(B
  user1.SetPrefix("/prefix");
  //$BMW5a$7$?$$%G!<%?$NL>A0$r@_Dj(B
  user1.SetAttribute("Frequency",StringValue("300"));
  //$BMW5a$9$kIQEY$r@_Dj(B
  NodeContainer UserNodes_Verification;
  for(NodeList::Iterator i=NodeList::Begin();i!=NodeList::End();++i){
    if(*i!=nodes.Get(ROUTER1_NUM)&&*i!=nodes.Get(ROUTER2_NUM)
        &&*i!=nodes.Get(SERVER_NUM)){
      UserNodes_Verification.Add(*i);
    }
  }
  user1.Install(UserNodes_Verification);
  
  //$BCY1d$rB,$k$?$a$N%N!<%I(B
  // ndn::AppHelper user2("CustomConsumer1");
  // user2.SetPrefix("/prefix/sub");
  // // user2.SetAttribute("Frequency"
  // // ,StringValue( std::to_string( SENDING_RATE ) ));
  // user2.SetAttribute("Frequency",StringValue("2"));
  // user2.Install(nodes.Get(0));
 
  //$B%f!<%6967b$J$7(B 
  // ndn::AppHelper user3("CustomConsumer2");
  //$B%f!<%6$N5sF0$r=q$$$?%*%V%8%'%/%H$r:n@.(B
  // user3.SetPrefix("/prefix");
  //$BMW5a$7$?$$%G!<%?$NL>A0$r@_Dj(B
  //user3.SetAttribute("Frequency"
  //,StringValue( std::to_string( SENDING_RATE ) ));
  // user3.SetAttribute("Frequency",StringValue("300"));
  //$BMW5a$9$kIQEY$r@_Dj(B
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
  //$B%k!<%?(B1
  ndn::AppHelper router1("CustomRouter2");
  //$B%k!<%?$N%*%V%8%'%/%H@8@.(B
  router1.Install(nodes.Get(ROUTER1_NUM));
  //$B%k!<%?G[CV(B

  //$B%k!<%?#2(B
  ndn::AppHelper router2("CustomRouter2");
  //$B%k!<%?$N%*%V%8%'%/%H@8@.(B
  router2.Install(nodes.Get(ROUTER2_NUM));
  //$B%k!<%?G[CV(B
  
  // $B%5!<%P(B
  ndn::AppHelper server("CustomProducer");
  //$B%5!<%P$N%*%V%8%'%/%H@8@.(B
  // serverHelper.SetAttribute("PayloadSize",StringValue("1024"));
  server.Install(nodes.Get(SERVER_NUM)); // $B%5!<%PG[CV(B
  
  //NDN$B$K$*$1$k%k!<%F%#%s%0=hM}$r:n@.(B
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
  
  //$B;XDj$7$?IC?t8e$K%7%_%e%l!<%7%g%s=*N;(B
  Simulator::Stop(Seconds(SIMULATION_TIME));
  // Simulator::Stop(Seconds(40.0 ));
  
  //NetAnim$B$N%U%!%$%k:n@.(B
  // AnimationInterface anim (animFile);
  //GUI$BI=<($N%U%!%$%k:n@.(B
  // anim.EnablePacketMetadata ();
  //GUI$BI=<($N%U%!%$%k:n@.(B
  // anim.SetConstantPosition(nodes.Get(0),0,78);
  // anim.SetConstantPosition(nodes.Get(1),35,35);
  // anim.SetConstantPosition(nodes.Get(2),35,0);
  // anim.SetConstantPosition(nodes.Get(3),71,78);
  
  //$B%7%_%e%l!<%?$r<B9T(B
  Simulator::Run();
  
  //$B%"%K%a!<%7%g%s%U%!%$%k:n@.40N;$r=PNO(B
  // std::cout << "Animation Trace file created:" 
    // << animFile.c_str()<< std::endl;
  
  //$B%7%_%e%l!<%?$rGK2u(B
  Simulator::Destroy();

  return 0;
}

//ns3$B$NL>A06u4V$N=*N;(B
}

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
