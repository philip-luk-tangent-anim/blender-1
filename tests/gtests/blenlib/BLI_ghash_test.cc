/* Apache License, Version 2.0 */

#include "testing/testing.h"

extern "C" {
#include "MEM_guardedalloc.h"
#include "BLI_utildefines.h"
#include "BLI_ghash.h"
#include "BLI_rand.h"
#include "BLI_string.h"
#include "PIL_time_utildefines.h"
}

const char *words10k = "\
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam auctor ultrices purus tincidunt mollis. Vestibulum tincidunt imperdiet molestie. Vivamus posuere, risus ut mollis rutrum, lacus nulla mollis velit, consectetur auctor erat est in odio. Proin quis lobortis ex. Ut id quam lacus. Morbi ultrices orci quis sem suscipit tincidunt. Nullam ut molestie justo, vulputate placerat diam. Nunc tincidunt auctor venenatis. Phasellus placerat, odio ac dictum pretium, nisi odio tristique sem, sit amet hendrerit odio tortor eu felis. Duis placerat tristique neque, sit amet ornare nulla fermentum vel. Vivamus vitae rhoncus ante. Sed a dolor mauris. Nullam bibendum vehicula semper. Duis ut commodo nibh. Nulla sit amet eros feugiat, accumsan nisl a, ornare quam.\
In non magna orci. Curabitur finibus tempus semper. Aliquam fringilla arcu consectetur blandit vestibulum. Mauris mollis est arcu. Praesent pellentesque lacus bibendum massa commodo commodo. Aenean facilisis lobortis varius. Ut semper ullamcorper dui, at pellentesque felis. Duis accumsan sapien ut malesuada lacinia. Praesent elementum venenatis arcu in mattis.\
Nunc sagittis mauris risus, quis rutrum nisi egestas quis. Maecenas pharetra posuere auctor. Suspendisse mollis sollicitudin elit, id cursus massa bibendum eu. Integer tincidunt dolor non porttitor tempus. Donec lacinia sapien eu enim feugiat suscipit non malesuada diam. Suspendisse nec convallis elit. Nulla eu augue ultrices, consequat lorem at, malesuada magna. Aliquam sed tempor ipsum. Sed hendrerit nec lectus et pharetra. In felis sem, cursus at nunc in, tristique convallis purus. Praesent augue turpis, porttitor consequat risus ornare, laoreet commodo dui. Nulla congue ultrices sapien a cursus. Nulla facilisi.\
Integer lacinia enim sodales sem mattis, sit amet egestas lectus tincidunt. Ut quis nisl ut ex luctus fermentum quis et diam. Maecenas lectus leo, hendrerit eu facilisis et, mattis ut sem. Duis imperdiet nisl vitae urna consequat suscipit. Suspendisse sed viverra massa, dapibus condimentum sem. Morbi suscipit congue odio. Nullam eleifend fringilla nisl et semper. Sed eu neque ante. Sed eget viverra urna.\
Duis tempor laoreet interdum. Nunc fringilla aliquet urna sit amet commodo. Curabitur non orci nec libero egestas ullamcorper nec nec velit. Nam vitae ligula lobortis, vehicula nulla id, lacinia urna. Morbi id dignissim eros. Etiam eu risus in sem vestibulum dapibus ut mollis sem. Quisque ultricies pulvinar maximus. Proin risus turpis, auctor eget molestie nec, molestie a ipsum. Donec dapibus dui in lorem rhoncus, non rutrum neque convallis. Donec at tincidunt turpis, nec scelerisque lorem. Donec ac sapien mi. Sed commodo efficitur tempus. Maecenas eu lobortis diam. Phasellus enim nulla, ornare ac laoreet egestas, vestibulum ac arcu.\
Pellentesque ultrices mauris sem, a iaculis diam tristique id. Proin sed facilisis mauris. Aliquam nibh ex, varius in consequat laoreet, sollicitudin id diam. Vivamus semper ultrices sem non tempor. Sed hendrerit maximus malesuada. In ex orci, elementum non magna eget, congue sagittis tellus. Donec malesuada sem leo, quis malesuada risus blandit et. Praesent porta malesuada metus eget pretium. Vestibulum venenatis tempor tellus at varius. Donec mauris arcu, elementum vitae aliquet nec, ullamcorper vitae neque.\
Nunc eu viverra justo, sit amet viverra elit. Proin urna elit, luctus ut placerat quis, blandit vitae diam. Vestibulum id fringilla enim. Ut eleifend augue ante, ac euismod sapien luctus sit amet. Pellentesque mattis tortor ac rutrum malesuada. Sed et nulla id metus faucibus condimentum. Vestibulum cursus posuere vestibulum. Proin auctor arcu erat, quis porta sem dignissim a. Donec sed finibus ante. Integer porttitor pretium nunc, eu semper elit.\
Nam sit amet ornare urna. Suspendisse porta augue id massa luctus maximus. Fusce tellus ligula, finibus sed lacus eget, tristique mollis libero. Vivamus velit diam, faucibus vel fringilla vitae, ornare id lacus. Pellentesque vel sem quis nunc semper porta ut sit amet sapien. Integer nec leo at tortor ullamcorper pulvinar at ut ante. Fusce velit nisl, fermentum in tempus ac, gravida ac tellus. In aliquet sollicitudin erat, non vestibulum diam aliquam in. Duis purus justo, aliquet ut libero vel, egestas mollis nibh. Praesent sed tempor mauris, vel tempor augue. Morbi eu eros vel velit condimentum porttitor nec sit amet odio. Nunc suscipit risus at ex aliquam, in pretium mi maximus. Mauris sollicitudin sit amet arcu luctus maximus.\
Curabitur vehicula condimentum porta. Nunc consequat vitae urna vel gravida. Vivamus vitae mattis augue, sit amet blandit enim. Phasellus odio leo, cursus eget lacus sit amet, facilisis mattis tortor. Duis venenatis ante libero, eu condimentum urna viverra fermentum. Suspendisse libero leo, pretium eu leo at, imperdiet ultricies nunc. Fusce ante neque, feugiat id lacus sed, fringilla suscipit ligula. Phasellus cursus malesuada urna, vel ullamcorper massa suscipit vitae. In eu bibendum augue. Duis auctor posuere turpis nec vestibulum. Vestibulum nec dui in mi consequat auctor sed at nisl. Suspendisse tellus elit, congue ut facilisis vel, ornare id mauris. Integer rutrum fermentum neque, vitae pharetra metus consectetur in. Duis vitae lacus scelerisque, rhoncus nisl id, sagittis elit.\
Praesent lacinia libero ac ultricies tempus. Etiam ut maximus sapien. Maecenas sit amet ante auctor, feugiat diam non, vulputate diam. Nulla facilisi. Vestibulum id augue velit. Donec at elementum urna. Morbi elementum nunc in neque ornare, sit amet tempor mauris vulputate. Nunc mauris mauris, lobortis non nibh sed, gravida sollicitudin nunc. Nunc vel dolor non augue venenatis semper vitae non turpis.\
Praesent mattis elit eu interdum porttitor. Etiam quis magna magna. Praesent a ipsum est. Aenean at ligula vel leo faucibus pulvinar sed eget mauris. Nam accumsan blandit nibh, nec tincidunt nisl eleifend sit amet. Etiam ornare, arcu nec dictum volutpat, nulla orci porttitor orci, vel venenatis mi massa at erat. Maecenas eget accumsan nisl, quis ullamcorper turpis. Pellentesque sit amet mi aliquet, feugiat felis in, dictum urna. Cras nulla leo, congue vel consequat gravida, aliquet a nulla. Nulla commodo, nisi eu ultricies feugiat, justo velit tempor ligula, a tincidunt nisi tellus ut sapien. Sed eget ornare magna.\
Cras ut vehicula sapien. Quisque id malesuada urna, vitae congue ante. Donec nec leo pretium, finibus nibh a, porta lectus. Fusce arcu tellus, tempor semper sem id, aliquam fringilla ipsum. Ut massa ante, placerat quis sapien quis, sollicitudin blandit turpis. Aenean posuere ullamcorper massa. Nam faucibus egestas arcu. Vivamus vehicula auctor diam, eu placerat diam ullamcorper at. Nulla eu consequat elit, vel semper turpis. Curabitur rhoncus nunc vel vestibulum interdum.\
Nam augue neque, pharetra vel nisi dignissim, vehicula dapibus risus. Cras eget mattis nisi. Sed tempor posuere gravida. Proin sagittis a nisl eget gravida. Curabitur viverra dapibus arcu, sit amet rutrum nibh fringilla euismod. Donec vitae risus non lorem facilisis cursus eu eu quam. Donec quis lacus blandit, consectetur elit ut, sagittis ligula. Etiam dapibus ex sit amet elit commodo finibus. Suspendisse non finibus felis, non cursus libero.\
Vivamus semper aliquet velit vel elementum. Phasellus dictum, tortor id sagittis ultrices, ex dui porttitor tortor, nec mattis dolor sem nec mi. Ut aliquam consequat eros sit amet mollis. Nullam mollis venenatis porttitor. Donec sit amet velit at velit luctus auctor dictum in neque. Ut vulputate ultricies mollis. Pellentesque elementum augue dolor, non varius ligula tristique ac. Nullam eget mauris urna. Integer elementum eleifend pulvinar. Morbi gravida ante eget ornare faucibus. Mauris pulvinar consequat nunc vel accumsan. Curabitur egestas urna elit, ut accumsan magna dictum in. Nam neque mi, ornare sed leo at, tempor vulputate nunc.\
Nunc dignissim mauris id dui iaculis fringilla. Praesent malesuada tellus in dapibus feugiat. Vivamus posuere, nisi et consequat euismod, lorem augue iaculis velit, eget iaculis neque quam eu mi. Nullam ac hendrerit felis, non elementum ipsum. Aliquam erat volutpat. Proin vel molestie felis. Nullam luctus vel ante nec facilisis. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.\
Duis et metus justo. Interdum et malesuada fames ac ante ipsum primis in faucibus. Ut tristique sit amet elit et congue. Aenean quis elementum enim, vitae pharetra sem. Vestibulum vel finibus nisl, at consequat eros. In vitae mollis lacus, et pharetra elit. Mauris varius sapien quis tincidunt blandit. Proin non semper nibh. Aliquam non elit id felis laoreet interdum eget a risus. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Suspendisse nisl tellus, mollis id erat vel, hendrerit volutpat nunc. Quisque scelerisque cursus tellus, nec placerat quam imperdiet in.\
Sed porttitor arcu vel ligula finibus, a vestibulum enim ultrices. Fusce imperdiet augue eget est vehicula porttitor. Quisque convallis odio vitae lorem porttitor iaculis. Ut dictum velit ac tortor lobortis ultrices. Vestibulum tincidunt vestibulum mauris, at fermentum elit imperdiet nec. Nunc finibus ornare lorem vel malesuada. Praesent arcu turpis, pulvinar sit amet accumsan quis, tincidunt vel justo. Pellentesque volutpat nec enim sit amet pulvinar. Nam eu libero dignissim, volutpat elit ut, semper tortor. Morbi pellentesque nisl lectus. In vel tellus sed sem luctus lobortis ut nec diam. Phasellus id semper sem. Phasellus in purus consequat, rhoncus mi mollis, finibus ligula. Fusce feugiat dictum consequat. Mauris egestas, est ut euismod consequat, arcu dui dignissim quam, pharetra dignissim orci dolor quis nisl. Nunc dapibus blandit urna non feugiat.\
Suspendisse non maximus augue. Quisque ut orci aliquet, vulputate massa eget, mattis diam. Etiam efficitur consectetur viverra. Nulla massa augue, elementum at turpis et, cursus ultricies risus. Suspendisse vel nibh placerat, imperdiet elit et, viverra ligula. Donec lorem lorem, hendrerit nec aliquam sit amet, scelerisque sit amet massa. Mauris convallis ullamcorper tortor sed malesuada.\
Fusce ultricies a turpis eu ornare. Suspendisse potenti. Sed non nulla condimentum, vulputate nisi nec, tincidunt arcu. Morbi erat leo, lobortis id odio ac, hendrerit sodales sem. Ut malesuada, lectus at posuere molestie, orci metus vehicula justo, mattis tincidunt arcu risus quis odio. Fusce non sem sed nisi consectetur finibus vitae quis diam. Vivamus a lacinia nisl. Praesent tempus nunc gravida, lacinia lacus in, lobortis massa. Aliquam gravida consequat nisi at fringilla. Quisque tortor tortor, tincidunt cursus lorem eget, ultrices ultricies lacus. Phasellus mattis iaculis elit, eget mattis nisl bibendum sed. Integer faucibus gravida nisl, ac consequat ex tempor at. Sed tempus elementum vestibulum.\
Suspendisse vitae enim semper, pulvinar diam eget, suscipit turpis. Maecenas ultricies, diam sed consectetur sagittis, diam sem cursus nisl, nec aliquet tellus augue quis ipsum. Cras vel lorem convallis, mattis risus at, placerat massa. Curabitur vel rutrum ligula. Quisque in nibh libero. Pellentesque diam tellus, consectetur eget quam ac, faucibus imperdiet odio. Sed tortor nulla, scelerisque non turpis nec, fringilla bibendum est.\
Etiam a urna eget erat tincidunt ultrices. Maecenas lorem odio, volutpat nec ligula id, hendrerit aliquam nulla. Aenean congue lacinia fermentum. Suspendisse sed interdum lacus. Fusce scelerisque posuere sagittis. Ut at semper tellus. Donec condimentum orci nunc, non fermentum purus volutpat eget. Maecenas elementum dapibus ante, eu suscipit quam imperdiet ut. Integer non congue elit. Sed venenatis, turpis varius commodo euismod, libero magna fringilla lacus, quis venenatis velit lectus sed augue.\
Morbi gravida orci odio, ut ornare massa sollicitudin a. Donec convallis mi et sapien tempor, non dapibus dolor fringilla. Aenean euismod rutrum turpis, et facilisis orci porttitor eu. Suspendisse in neque leo. Nulla facilisi. Etiam mollis orci nisl, quis scelerisque metus efficitur vehicula. Nam porta molestie tortor, sit amet consectetur leo vestibulum vel. Pellentesque a volutpat augue. Maecenas vel elementum ex, eget elementum leo. Curabitur at maximus metus, quis porttitor orci. Praesent auctor commodo elit, a dapibus tortor volutpat et. Praesent dictum posuere dolor sit amet molestie. Sed viverra augue nec eros mattis blandit. In quis sodales dolor. Donec sed purus ex. Fusce erat magna, efficitur ac tempus ac, lacinia quis augue.\
Aliquam porta efficitur est vel placerat. Phasellus egestas vel nunc eu consequat. Maecenas ligula arcu, molestie ut dui ut, ornare finibus felis. Duis condimentum non augue ut posuere. Aenean mattis eros ut ligula ornare finibus. Aliquam feugiat ut turpis a feugiat. Vestibulum eget sollicitudin orci, nec fermentum justo. Praesent efficitur est a metus bibendum, eget feugiat diam suscipit. Suspendisse sit amet ipsum ut purus feugiat pretium. Morbi nisl risus, ultricies sit amet ullamcorper euismod, commodo eu libero. Aenean fringilla ipsum nec orci rutrum aliquet. Aenean lacus ante, eleifend eu eleifend fringilla, elementum ac justo. Vestibulum tincidunt interdum lectus sit amet fermentum. Etiam rhoncus eu ante lacinia sagittis. Maecenas iaculis ut erat quis feugiat. Maecenas sed est vel tellus bibendum rutrum volutpat nec odio.\
Vivamus euismod augue nec purus euismod, mattis finibus nisi finibus. Donec quis ultrices massa. Quisque at nisl faucibus, facilisis tellus ut, ultricies dui. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Donec et arcu eros. Etiam dapibus bibendum felis eu viverra. Integer a lacus venenatis elit lacinia facilisis non non felis. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ultricies augue at sapien mattis aliquam. Quisque nec semper purus. Cras auctor aliquet lacus, sed facilisis urna sollicitudin non. Vivamus eget erat purus. Sed a risus augue. Donec non sem sed sapien accumsan lacinia.\
Ut mauris odio, vehicula id accumsan at, tincidunt non odio. Nunc porttitor luctus ante ac cursus. Cras et dapibus ex, id pretium ligula. Proin volutpat rhoncus ex vitae venenatis. Pellentesque imperdiet, magna non tempus auctor, metus dolor scelerisque dui, id tempor purus est in risus. Suspendisse vehicula imperdiet sapien, nec pulvinar dolor ornare ac. Nulla luctus, nisl in aliquam blandit, risus orci placerat nunc, id tempus sem neque vitae leo. Aenean at elit elit. Suspendisse finibus dictum interdum.\
Nunc consectetur eget quam vitae egestas. Pellentesque tellus augue, aliquet at faucibus ac, imperdiet ut nulla. Maecenas quis lorem velit. Donec porta ligula et suscipit luctus. Aliquam sed pretium nunc. Nunc quis posuere tortor. Fusce in lectus nec turpis rhoncus pellentesque eu at quam. Nulla facilisi. Sed ante nulla, posuere ac ullamcorper vel, rhoncus vitae nisl. Nam non pellentesque arcu. Vivamus nibh leo, pellentesque a mollis non, gravida ut erat.\
Donec purus urna, pulvinar eu iaculis blandit, rutrum eget nulla. Fusce quis fermentum diam, faucibus volutpat lorem. Maecenas aliquet nisi nisl, eget sollicitudin ipsum facilisis at. Mauris nec sapien nisi. Duis ac laoreet sapien, a condimentum nisi. Nam vitae sapien sed sem convallis ornare. Pellentesque neque diam, ullamcorper et dolor sit amet, faucibus venenatis tortor. Nunc vel erat malesuada, vulputate odio sit amet, aliquam dui.\
Donec tincidunt arcu ut risus laoreet, id malesuada leo ultrices. Praesent a scelerisque libero, vitae suscipit massa. Quisque faucibus mauris rhoncus turpis vestibulum rhoncus. Donec vel molestie magna. Aenean et lorem dui. Nam iaculis ante sapien, semper tincidunt tortor hendrerit id. Nulla sed orci mi. Aliquam hendrerit libero erat, ac aliquam massa rutrum non. Suspendisse eleifend, elit in aliquet hendrerit, tellus erat sodales neque, quis rhoncus tellus sem vitae est. Interdum et malesuada fames ac ante ipsum primis in faucibus. Etiam quis mauris non ipsum tristique interdum sit amet eget mi. Ut velit risus, gravida ut efficitur sit amet, commodo at diam. Sed consectetur dui porttitor quam feugiat, et auctor mauris maximus. Nullam lobortis ac mi lacinia egestas. Proin ante massa, malesuada ut nulla elementum, venenatis mollis ante. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.\
Mauris eget gravida eros, non varius velit. Integer consectetur lectus nec arcu scelerisque, scelerisque vulputate mauris suscipit. Aliquam orci dui, faucibus et rutrum in, rhoncus quis dolor. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Maecenas ante nunc, placerat id lectus sit amet, luctus cursus ante. Nulla nec placerat arcu. Fusce ac dictum ex. Vivamus semper nulla vitae neque volutpat, auctor vestibulum arcu tempus. Pellentesque aliquam tincidunt arcu, et pharetra neque. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Nunc risus augue, malesuada quis risus a, suscipit semper metus. Suspendisse ac rhoncus felis. Aliquam orci lectus, elementum at nulla at, ullamcorper pellentesque leo.\
Quisque nisi tellus, pharetra in pellentesque in, facilisis vitae velit. In ex ex, sagittis at dolor vel, congue ultricies velit. Duis quis gravida mi. Aenean tempor efficitur lectus. Fusce sodales, ex eu efficitur iaculis, metus sem eleifend purus, ut commodo arcu tortor eget urna. Etiam nisi nisl, malesuada convallis ex at, malesuada elementum nunc. Vivamus commodo mi id ligula tincidunt posuere. Integer eget arcu cursus, sagittis quam eu, aliquam leo. In auctor eget mauris et elementum.\
Aenean sagittis euismod tellus sed accumsan. Aliquam erat volutpat. Aliquam erat volutpat. Ut consectetur porta ipsum sit amet porttitor. Nam ut nunc a turpis auctor finibus. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Donec non nisl condimentum, fermentum augue in, egestas libero. Pellentesque ut odio rhoncus, sollicitudin felis vitae, pellentesque est. Suspendisse tincidunt eros eget ex vestibulum elementum. Vivamus mollis scelerisque diam, quis dignissim dolor venenatis at. Ut gravida sapien vitae risus efficitur, ut auctor justo gravida. Cras arcu elit, interdum vel purus sit amet, venenatis molestie tellus. Integer consectetur tempor velit a varius. Praesent congue, massa non congue blandit, tortor purus imperdiet elit, sit amet pharetra arcu lacus egestas neque.\
Maecenas in erat arcu. In varius, risus vitae mollis sodales, nisi velit bibendum tortor, vitae sagittis augue tortor quis nunc. Fusce posuere dolor ac tincidunt facilisis. Phasellus in lacus diam. Fusce mattis sapien tellus, scelerisque pharetra leo eleifend nec. Cras libero diam, convallis in luctus a, iaculis a ipsum. Duis arcu leo, volutpat non mauris et, scelerisque suscipit diam. Ut vulputate placerat velit quis placerat. Duis commodo non turpis et convallis. Duis nec pulvinar metus, ac tristique leo. Fusce vehicula augue ac placerat elementum. Nulla dapibus nisi pretium lectus sodales, ac congue sapien ornare. Vestibulum sagittis orci ut purus efficitur, eu mollis libero placerat.\
Vestibulum ullamcorper odio non quam mollis, eget rhoncus metus eleifend. Mauris scelerisque, massa rutrum sodales malesuada, elit dolor blandit lectus, quis faucibus felis odio feugiat lacus. Nunc bibendum congue efficitur. Nunc a purus neque. In lobortis metus nisi, vel pellentesque mi facilisis sed. Donec in pretium neque, in maximus metus. Integer faucibus diam sed tristique sagittis. Nullam eget maximus leo, eget malesuada leo. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Aenean porttitor risus eget eros euismod molestie. Integer tristique tincidunt elit, non posuere libero pretium vel. Fusce dapibus, nisi nec egestas dapibus, lectus arcu maximus leo, a finibus diam arcu ut mauris.\
Vivamus tincidunt lectus ut augue ultrices, et cursus sem cursus. Proin in quam mauris. Maecenas vel magna dapibus, interdum ipsum mattis, posuere tortor. Cras eu massa ex. Donec eget massa vel dui gravida luctus vel a quam. Etiam eu lobortis neque. Etiam ligula dui, dictum ut turpis ac, eleifend pretium turpis.\
Vestibulum convallis finibus commodo. Morbi fermentum ante nunc, a rhoncus lacus ultricies quis. Suspendisse finibus quam blandit odio elementum, non efficitur diam laoreet. Cras aliquet ligula eget magna scelerisque, ut ornare nisi elementum. Duis nisl massa, suscipit id nibh a, venenatis auctor risus. Nulla luctus eget odio quis ultrices. Etiam consequat sapien ut nisl mollis cursus. Pellentesque a lacinia odio, id varius lorem. Curabitur scelerisque in urna eget pretium. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos.\
Sed leo metus, fermentum vitae quam ut, suscipit efficitur purus. Sed facilisis dapibus pulvinar. Cras sed eleifend mi. Sed quis nibh in sapien venenatis interdum ac nec orci. Sed non tortor urna. Nam rutrum lacinia diam id vehicula. Quisque vitae lobortis nibh, at tempor purus. Suspendisse dictum interdum nisi, quis maximus ipsum commodo tempus. Nulla semper congue gravida. Aenean at nibh in eros aliquam egestas. Nulla fermentum efficitur laoreet. Donec non lorem nec augue porttitor cursus eu in quam.\
Aenean laoreet quam neque, at tempus nisi ultrices id. Quisque in diam lacinia nulla scelerisque rhoncus vitae eget nulla. Donec vel est metus. Nullam suscipit odio eu enim lacinia facilisis eget in tellus. Vestibulum vehicula risus nec odio consectetur, a cursus massa imperdiet. Duis facilisis felis quis nunc mattis, nec volutpat libero tempor. Nulla nec leo sed tellus maximus lobortis. Suspendisse at urna nibh. Vestibulum eget turpis nisl. Donec scelerisque neque auctor erat tempor elementum sed id lacus. Sed metus nulla, dictum non luctus vel, suscipit et ex. Quisque laoreet sapien non neque iaculis, at aliquam massa viverra.\
Nullam nibh diam, imperdiet eu nunc sed, congue cursus leo. Morbi tristique diam metus, at faucibus magna mollis at. Sed eget nibh nunc. Nam nec elementum sem, sit amet tincidunt lorem. In viverra elit et interdum fermentum. Integer imperdiet orci ac justo molestie ullamcorper. Pellentesque fringilla tortor erat, scelerisque maximus nisl sollicitudin a. Integer nisi elit, pharetra eget lacinia non, congue sit amet ex. Phasellus tempus suscipit ultrices. Quisque ac nibh dignissim erat bibendum cursus vel a enim. Curabitur a augue sit amet lorem pharetra feugiat.\
Donec euismod, massa at venenatis bibendum, elit libero pellentesque velit, eget congue metus risus a enim. Aenean pretium vestibulum enim, sit amet vulputate urna auctor vitae. Praesent porttitor erat eu mi cursus venenatis. Maecenas ut ultrices neque, ac feugiat libero. Nulla finibus sit amet sem in auctor. Nam fermentum maximus ex, et consequat velit lobortis id. Aliquam eu feugiat est. Donec quis leo ex. Suspendisse convallis eget nulla eu aliquet. Quisque aliquet tortor vitae ipsum fermentum tristique. Sed convallis rutrum augue, ac viverra est pharetra quis. Ut porttitor magna massa, placerat maximus lectus scelerisque quis.\
Sed viverra urna in neque feugiat rhoncus. Donec ut viverra odio, laoreet dignissim dui. Aenean tristique feugiat diam vel luctus. Cras sit amet condimentum neque, ut faucibus ante. Aenean vitae elit id est laoreet efficitur in sit amet magna. Praesent ante felis, blandit id nisl ut, porta fringilla orci. Aenean vel accumsan metus, vel vehicula metus. Nulla placerat nibh et auctor convallis. Maecenas magna metus, pretium ac sodales ac, eleifend quis eros. Praesent volutpat quam a pulvinar pharetra. Sed arcu dolor, aliquet nec magna in, faucibus consequat lorem.\
In tincidunt, ex a finibus rutrum, metus dui fringilla ex, ac mollis elit leo eget augue. Nunc vehicula facilisis nibh, quis ultricies sem. Praesent nulla est, finibus in lorem in, mattis placerat urna. Proin hendrerit risus nunc, id congue ex posuere id. Aenean ullamcorper tortor quis lorem consectetur, et euismod leo fermentum. Praesent vulputate congue lectus sit amet pulvinar. Vestibulum vel vestibulum quam, in convallis diam.\
Maecenas sollicitudin magna odio, eget mollis purus posuere eu. Curabitur molestie mattis ligula, a maximus dui fermentum ut. Fusce justo velit, eleifend ut tellus vitae, volutpat maximus risus. Pellentesque suscipit mauris non purus placerat porta. Nunc in malesuada mi, vel bibendum felis. Aenean pretium nunc id efficitur porttitor. Mauris malesuada, tortor sit amet blandit tincidunt, tellus est ullamcorper diam, sit amet aliquet ex velit interdum quam. In hac habitasse platea dictumst.\
Sed vitae est eu elit posuere mattis nec a mauris. Morbi id ligula sed nunc sagittis finibus vitae eu nisi. Cras dignissim sagittis tellus a suscipit. Nunc semper erat nec libero vestibulum, at mattis purus scelerisque. Pellentesque egestas volutpat eleifend. Nullam venenatis erat id diam venenatis, sed rhoncus felis hendrerit. Nullam luctus facilisis risus. Mauris sed urna nisi. Ut tempus feugiat metus. Integer at purus velit. Praesent neque felis, pellentesque vitae sem nec, tempor commodo urna. Morbi malesuada ante sit amet purus tincidunt pellentesque. Aenean commodo lectus sit amet dignissim hendrerit. Phasellus auctor tellus ligula, eu ultrices ex egestas non. Mauris eget nisl dictum, scelerisque sapien et, dapibus felis. Aenean in dignissim leo.\
Sed semper, ex at euismod molestie, ex odio ullamcorper nisi, et facilisis lectus eros non magna. In hac habitasse platea dictumst. Pellentesque sed maximus mauris. Cras luctus dapibus nunc, sit amet suscipit dui viverra nec. Donec gravida tortor porttitor orci malesuada porttitor. Nunc condimentum eu libero sit amet varius. Curabitur mollis urna eu porta tincidunt. Nullam ultricies magna libero, et dapibus tellus tempus eu. Nullam pretium lectus nec iaculis pretium. Maecenas at arcu lobortis, ornare ante nec, euismod metus. Pellentesque volutpat tellus nulla. Aenean mattis efficitur velit vitae blandit. Duis vel egestas eros. Pellentesque aliquam placerat elit, eu congue sem ullamcorper sit amet. Ut erat nisl, luctus vitae pellentesque ut, tristique eu odio.\
Pellentesque nec fermentum ex, rhoncus varius dui. Mauris lobortis nunc nec dui volutpat consequat. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Aliquam dignissim purus sed ligula pretium placerat. In vestibulum ultricies mauris. Curabitur aliquet lorem quis libero auctor, ut rhoncus purus lobortis. Nulla elementum erat nec rhoncus posuere. Integer faucibus quam sed elementum fringilla. In in lobortis sapien, nec commodo tortor. Aenean euismod ipsum nisi, vitae fringilla leo imperdiet ut. Donec a semper odio, et tempor magna.\
Cras cursus vel augue quis egestas. Nam nec tortor blandit, mattis quam imperdiet, finibus quam. Pellentesque tincidunt eros urna, ut tristique diam faucibus condimentum. Ut dictum risus mi, non sollicitudin turpis facilisis sit amet. Morbi finibus scelerisque mattis. Fusce vel tempor purus, nec pharetra augue. Curabitur dapibus, orci eu consectetur ultrices, diam mauris sodales urna, non euismod diam lacus luctus risus. Mauris commodo accumsan sapien. Proin vel blandit sapien. Donec porta tortor vel nibh faucibus molestie.\
Pellentesque placerat justo erat, vitae tristique felis fringilla eget. Quisque facilisis justo at orci lobortis, ut commodo diam egestas. Etiam non tristique nisl. Cras varius, massa a sollicitudin ornare, turpis arcu fringilla leo, non euismod ligula arcu id lacus. Suspendisse potenti. Morbi pharetra dolor eget porta tristique. Nullam sem tortor, lobortis eget hendrerit a, efficitur sit amet sapien. Fusce sit amet condimentum odio, aliquet rutrum velit. Morbi vel rhoncus ante. In blandit eros ut lectus varius, quis tempor arcu iaculis. In massa leo, venenatis nec lobortis non, pulvinar non nunc. Nunc vehicula, erat vitae placerat eleifend, eros ipsum consectetur odio, eu ornare velit mauris nec sapien. Integer a consequat libero.\
Quisque velit augue, blandit eu luctus sit amet, laoreet sit amet odio. Etiam in enim lacus. Interdum et malesuada fames ac ante ipsum primis in faucibus. In rutrum a tortor id pulvinar. Donec pretium lorem sed sem eleifend fringilla. Fusce sollicitudin ac ligula eget pharetra. Sed cursus diam non sem ullamcorper efficitur. Vivamus congue ligula iaculis justo iaculis elementum. Integer tempor nisl arcu, ut tincidunt erat vestibulum et. Suspendisse rutrum aliquet eros non pellentesque. Mauris laoreet, diam id tincidunt faucibus, risus velit venenatis risus, in venenatis justo diam et orci. Etiam pulvinar pulvinar nisi, id efficitur erat vulputate ut.\
Sed suscipit sodales ante, a blandit orci maximus vel. Vestibulum at aliquet orci. Proin tincidunt nisi quis eros consequat consectetur. Praesent congue lobortis laoreet. Donec imperdiet risus erat, eu volutpat justo posuere id. Fusce placerat sollicitudin eros vitae tincidunt. Sed orci ante, ultricies sed dapibus vel, sagittis ac massa. Pellentesque vel mauris nec est hendrerit posuere. Integer sagittis diam sed felis facilisis ultrices. Aliquam erat volutpat.\
Nulla pharetra justo in ipsum dapibus, nec viverra nunc euismod. Nulla massa ante, euismod at interdum vel, dapibus ut ex. Etiam consequat mauris a suscipit lobortis. Donec commodo convallis velit, eget commodo urna vulputate ac. Sed molestie vel dui ut feugiat. Donec orci purus, placerat vitae egestas sed, sodales nec ex. Sed egestas turpis non malesuada semper.\
Donec et mi a nisi volutpat sagittis. Suspendisse potenti. Phasellus mollis sapien ac tellus imperdiet tempus. Praesent nec sapien sit amet ipsum interdum interdum non eget nunc. Aenean fringilla lorem a viverra rutrum. Donec at maximus nibh. Phasellus facilisis justo sit amet metus pharetra sagittis. Quisque mollis metus laoreet ipsum tincidunt sollicitudin. Maecenas sit amet dictum ligula. Fusce molestie iaculis dui, et gravida libero hendrerit in. Praesent euismod libero metus, vitae rhoncus velit ultrices eget. Vestibulum ac massa bibendum, gravida dolor vel, dapibus est. Etiam non elit varius, mollis purus eget, placerat velit.\
Nullam lectus dui, mattis at pulvinar eu, elementum et lorem. Sed vel auctor orci, nec semper neque. Nullam cursus commodo quam, in ultricies tellus rhoncus vulputate. Mauris dapibus ipsum ipsum, dapibus euismod purus pellentesque at. Nullam euismod lectus non risus consequat vulputate. Quisque finibus a turpis eu convallis. Nam magna turpis, feugiat ut urna in, tempus facilisis elit. Duis dignissim purus sagittis porttitor posuere. Suspendisse varius ligula at egestas scelerisque. Duis placerat sagittis nisi, et molestie tortor posuere condimentum. Morbi hendrerit, ante ornare tempus finibus, ex ipsum laoreet dui, vel ornare felis tortor sit amet metus. Vivamus laoreet placerat massa, non suscipit nisl faucibus eget. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae;\
Pellentesque orci lacus, vulputate cursus ex eu, porta aliquam massa. Proin dolor massa, faucibus vel rhoncus et, venenatis a nisi. Vivamus venenatis enim mi. Sed viverra augue vitae lectus lobortis vulputate. Phasellus ac ligula congue, sagittis est non, aliquet tortor. Suspendisse faucibus euismod neque, ac congue felis. Curabitur maximus neque sit amet odio varius gravida. Proin egestas nulla eget mi bibendum luctus.\
Ut non mollis mi. Quisque finibus, eros non lobortis interdum, diam nisi faucibus diam, non imperdiet leo velit et dolor. Nam est massa, vehicula sed diam sed, laoreet convallis nisi. Donec enim ligula, dignissim non sem ut, pulvinar cursus mi. In at dignissim nulla, ac fringilla neque. In hac habitasse platea dictumst. Quisque luctus mattis orci, consequat egestas nisi. Vivamus et metus et quam porttitor elementum. Suspendisse auctor mauris eu sollicitudin sagittis. Vivamus ac ante non augue iaculis consectetur quis quis dui. In bibendum risus tristique ligula iaculis finibus. Phasellus non ante risus. Maecenas ac leo cursus, molestie purus sed, tristique purus. Etiam sem nulla, aliquam nec laoreet nec, iaculis quis nulla.\
Maecenas id dui id neque venenatis gravida. Etiam vestibulum felis at diam porta ultrices. Ut finibus tortor et augue ornare, et efficitur purus scelerisque. Phasellus et ultricies arcu, vel lacinia lacus. Aenean tincidunt eleifend nunc, sit amet mattis purus venenatis sit amet. Curabitur eleifend sem nisl, et feugiat diam pharetra sit amet. Mauris ullamcorper mi vel condimentum egestas. Nulla pulvinar purus vel sagittis posuere. Nulla quis enim bibendum, iaculis quam in, tincidunt quam. Vestibulum rhoncus volutpat risus. Nulla ultricies bibendum est non malesuada.\
Nunc porta erat est, a tincidunt magna gravida vel. Maecenas sit amet aliquet odio. Vestibulum egestas, tortor scelerisque consectetur pharetra, nisi tellus feugiat justo, et bibendum libero mi in diam. Aliquam tempus sapien nec tristique convallis. Nullam congue, lacus quis bibendum dignissim, nisl purus molestie dolor, a tempor dolor nibh pretium tellus. In hac habitasse platea dictumst. Cras at est turpis. Nam nec lacus posuere, mattis mi eu, viverra ex. Nullam eleifend ornare orci, vel tempor tellus. Ut nec eros eget tortor tempus tristique commodo sed lorem.\
Donec quis scelerisque nibh, non tincidunt velit. Fusce in eleifend sapien. Nunc sodales sem ut nunc pellentesque, eget pharetra justo tempor. Proin pretium velit et vehicula interdum. Maecenas luctus venenatis tincidunt. Donec hendrerit, ligula non volutpat porta, dui ante facilisis massa, at congue orci mi sed quam. Donec lorem ipsum, malesuada quis purus in, commodo malesuada justo. Etiam luctus, lorem vel rutrum tristique, mauris urna volutpat felis, a laoreet urna nunc et neque. Morbi a diam tincidunt augue cursus commodo nec ut ligula. Maecenas ultrices purus fermentum ullamcorper aliquet. Maecenas mi enim, semper nec metus at, posuere tristique ligula.\
Suspendisse est elit, porta quis massa id, gravida commodo ante. Nullam maximus mauris sit amet dolor tempus posuere. Phasellus purus mi, interdum in ipsum quis, tristique venenatis dolor. Suspendisse potenti. Phasellus odio erat, varius sed aliquet vestibulum, laoreet sed mauris. Vivamus sapien erat, maximus tristique elementum ac, eleifend in enim. Morbi accumsan elementum neque, a facilisis enim laoreet non. Donec auctor condimentum fringilla. Proin id urna nec tellus maximus maximus tincidunt et libero. Integer ultricies venenatis odio, ut volutpat odio laoreet non.\
Donec in scelerisque justo. Integer mauris libero, fringilla vel sapien sit amet, laoreet tincidunt dolor. Nam efficitur sagittis arcu, vel lobortis dui gravida non. Curabitur lobortis feugiat finibus. Vestibulum dictum tortor nec magna fringilla blandit. Nulla facilisi. Sed cursus laoreet neque vitae pulvinar. Ut iaculis euismod ullamcorper. Nunc in hendrerit lectus, sed venenatis mi. Suspendisse et est dui. Sed elementum augue non ornare cursus. Quisque varius facilisis magna nec laoreet. Suspendisse consequat, risus sed tempus egestas, velit felis faucibus erat, eu pharetra erat nisl sed turpis.\
Sed ultricies ac quam id mollis. In consequat et erat vitae interdum. Pellentesque malesuada feugiat ligula eu consectetur. Vestibulum tempor mi quis purus luctus dictum. Etiam condimentum ac ligula eget imperdiet. Ut placerat, tortor eu lacinia imperdiet, enim nibh aliquam nibh, quis faucibus enim odio eu arcu. Nullam sagittis, diam a ornare congue, ipsum eros scelerisque est, sit amet sagittis nisl tellus in felis. Nam eget ornare turpis. Sed tempor ac enim a vestibulum. Pellentesque eleifend lacus non libero accumsan, ut consectetur sapien lacinia.\
Etiam ut arcu non mi feugiat accumsan ut sit amet risus. Donec consequat eros sapien, malesuada imperdiet justo bibendum sit amet. Nulla pretium varius lectus, in eleifend quam fringilla in. Quisque eu pretium velit. Sed eget lectus sit amet tortor blandit tempus vel at sapien. Sed at velit porta, venenatis lorem sed, dapibus arcu. Donec pellentesque tortor id massa interdum pretium. Praesent id diam quis nunc dictum finibus quis quis ipsum. Quisque consectetur risus eu elit viverra, eget laoreet odio efficitur.\
In congue turpis iaculis ullamcorper bibendum. Duis at elit et velit varius vulputate ut ac turpis. Nunc posuere, urna id lobortis ornare, neque ex ultricies erat, id sollicitudin ante quam sed magna. Nunc ultrices quam erat, eget dictum libero sollicitudin in. Nulla facilisi. Pellentesque eleifend risus non justo imperdiet aliquet. Donec finibus auctor ornare. Duis in arcu lacinia, fermentum tellus vel, efficitur justo. Morbi nec nunc leo. Proin lacinia erat vel elementum dapibus. Proin diam ipsum, mollis eu lobortis a, facilisis consectetur est. Vestibulum rutrum pellentesque urna, a laoreet justo dictum vitae. Nullam dictum, mi elementum dictum interdum, sem nisl fermentum est, nec mattis enim ante aliquam tortor.\
Phasellus eu quam magna. Vivamus augue enim, dictum in nibh non, condimentum tristique lorem. Suspendisse potenti. Sed nibh lacus, auctor ut arcu sollicitudin, posuere tempor urna. Phasellus at odio euismod ipsum congue auctor. Fusce vestibulum elementum nunc, vel feugiat nibh bibendum at. Quisque felis ligula, fermentum a metus ac, pulvinar hendrerit est. Proin vitae tincidunt purus, vestibulum eleifend ipsum. Ut rhoncus et elit ut varius. Praesent eu pharetra tellus. Suspendisse varius, dui quis efficitur fermentum, est lectus ultricies ex, a fermentum orci nunc eu lorem.\
Integer aliquet nunc ullamcorper lacinia elementum. In cursus tortor nisi, ut pharetra tortor venenatis eu. Duis tincidunt, libero sed varius dictum, neque velit facilisis enim, eget bibendum mi eros et nisl. Nam turpis neque, lobortis eget ante ac, tristique congue lacus. Aenean dictum vitae tortor sed tristique. Donec sodales in arcu ut tristique. Curabitur in facilisis nisi, non vulputate odio. Phasellus ut fringilla nunc, nec dapibus turpis.\
Sed ut erat tempor sem vulputate gravida at at dui. Aenean id dolor ante. Morbi auctor interdum nisi, id pretium eros ultrices vel. Nulla eget justo fringilla, finibus quam et, accumsan ex. In nisl neque, pharetra nec volutpat at, mattis nec odio. Nam et sapien sed libero lacinia tempor sit amet vitae turpis. Praesent vel porta lacus, porta dignissim nunc.\
Aenean vitae vulputate purus. Ut at elit arcu. Integer risus neque, varius ac elit maximus, ultricies sagittis nisi. Pellentesque sapien magna, malesuada tincidunt ornare sed, malesuada tempor odio. Morbi id neque velit. Pellentesque at velit sed elit eleifend auctor. Quisque tincidunt tempus justo, venenatis dapibus sem pellentesque quis. Suspendisse finibus feugiat est id consectetur. Nulla commodo, massa auctor vulputate egestas, arcu massa tincidunt leo, quis ullamcorper sapien augue in nibh. Pellentesque ultrices ligula tincidunt urna fringilla, ac ultricies eros convallis. Ut nec massa diam. Maecenas justo nulla, dapibus id justo sollicitudin, fermentum tempor dui. Vivamus laoreet auctor mi non venenatis.\
Nulla commodo libero ac ex volutpat tincidunt. Donec vestibulum blandit purus bibendum laoreet. Morbi in porta orci. Nam commodo ex eget diam maximus cursus. Proin bibendum quis felis eget euismod. Praesent neque neque, pulvinar eu sem non, gravida ornare tortor. Ut tortor nisi, suscipit in lectus ac, volutpat pretium nisi. Nam rutrum nec dui quis vulputate. Duis in velit enim. Fusce porttitor vitae nisi a tincidunt. Ut enim purus, venenatis ut purus ut, iaculis dignissim ex. Aliquam erat volutpat. Suspendisse potenti. Maecenas ut malesuada elit. Maecenas tellus neque, pulvinar non metus ut, viverra finibus diam.\
Sed ac porttitor dui. Fusce sit amet ligula metus. Integer id aliquet libero. Sed tempor nisl in porttitor ultricies. Maecenas molestie orci sed sapien molestie interdum non id felis. Nullam sagittis elementum erat in pretium. Nunc pellentesque, ex sit amet fringilla dignissim, augue quam dictum leo, eget tristique turpis mauris sed metus. Praesent vel mauris risus. Etiam eleifend metus ut risus tempor, ac ultrices dolor dictum.\
Nulla sagittis non urna vitae feugiat. In venenatis arcu vel finibus volutpat. Nam non bibendum magna, nec eleifend ex. Etiam sit amet nisl euismod, mattis nisi quis, commodo nisl. Nunc eget mauris vulputate, cursus neque in, hendrerit ante. Cras non nisl in nisl laoreet aliquam. Sed vestibulum, nunc vitae molestie varius, lectus justo vehicula est, nec placerat ipsum lectus quis leo. Maecenas efficitur semper eros, sed pretium arcu blandit eu. Aliquam eget purus cursus, sollicitudin augue quis, cursus purus.\
Maecenas sed finibus ligula. Curabitur at diam quis eros mollis semper. Nulla commodo nisi libero, id feugiat nisl tincidunt bibendum. Mauris convallis tincidunt justo eu sodales. Quisque arcu lacus, finibus sed hendrerit at, convallis ut diam. Nulla enim nulla, efficitur quis tincidunt et, pulvinar sit amet enim. Aenean mattis urna id mauris maximus tincidunt. In hac habitasse platea dictumst. Morbi ornare porta congue. Aliquam hendrerit efficitur mi at aliquet.\
Vivamus rutrum lectus vel turpis volutpat, consectetur congue sem consectetur. Sed rhoncus elit sed orci tincidunt, ut condimentum diam ornare. Nulla facilisi. Ut placerat et massa nec malesuada. Praesent dapibus condimentum augue, at imperdiet lacus facilisis sed. Praesent at metus nunc. Morbi accumsan eros et turpis viverra, nec sagittis odio iaculis. Aenean rhoncus, nibh a consectetur sodales, massa lorem commodo dui, sit amet consequat ex arcu eget augue.\
Praesent quis nibh urna. Cras eu congue ligula, in ultricies ante. Etiam interdum, est tincidunt euismod sollicitudin, lectus felis mollis ex, pretium fringilla magna lorem non libero. Fusce aliquam tellus eget sodales commodo. Sed sapien lectus, dapibus quis elit at, ultricies tincidunt eros. Nulla suscipit orci sit amet aliquam pellentesque. Cras sed eleifend ligula, quis vehicula ligula. Integer quis tortor in mauris dictum malesuada sed non turpis. Nulla faucibus quis arcu molestie vulputate. Proin fermentum tellus feugiat, imperdiet mi sit amet, tempor sem. Mauris hendrerit augue a vulputate vulputate. Vivamus sagittis at odio non venenatis. Nunc a molestie dolor. Nunc erat nisi, consequat et tristique in, blandit non tortor. Vivamus euismod bibendum augue, ut aliquam lorem mattis quis.\
Duis laoreet odio at justo ultricies, nec scelerisque enim euismod. Sed eu turpis a lorem cursus feugiat. Duis ultrices molestie nulla non pharetra. Morbi faucibus est auctor faucibus placerat. Donec blandit quis ex ac pulvinar. Vestibulum a consequat quam. Fusce vitae facilisis ex. Etiam a risus eu orci tincidunt interdum. Proin interdum eros nec nibh venenatis, sed luctus sapien tincidunt. In cursus, ante nec dapibus bibendum, augue tortor venenatis felis, eu aliquam erat est vitae diam. Cras lacinia placerat quam, eu finibus purus. Aenean et augue purus. Praesent efficitur ornare magna in cursus. Nunc quis tempor ante, ac accumsan ligula. Nullam elit diam, tempus in sollicitudin at, fermentum tincidunt mi. Vestibulum accumsan, nisi at rutrum scelerisque, justo mauris cursus nulla, finibus cursus nulla elit quis augue.\
Aliquam lacus ante, ullamcorper quis varius vitae, ullamcorper eget magna. Phasellus mollis nisl eu nulla eleifend, non tempus tellus faucibus. Curabitur molestie eros id eleifend accumsan. Suspendisse tristique sem ante, non blandit eros accumsan ac. Ut sit amet ante justo. Nam condimentum felis quis urna sagittis hendrerit. Cras condimentum est ac massa aliquet finibus.\
Donec faucibus malesuada fermentum. Aliquam malesuada augue vitae dolor rutrum pellentesque. Nullam vulputate rhoncus porta. Quisque vulputate dignissim felis sit amet aliquet. Nam elementum odio velit, eget fringilla mi dignissim at. Mauris mollis diam orci, vel porta risus tempor a. Nullam quis dolor volutpat, ornare est at, fermentum urna. Fusce mollis nisl a augue condimentum, eu dictum dolor posuere. Mauris et egestas sem. Sed pretium lectus laoreet velit feugiat luctus. Nullam sodales at augue vel semper. Pellentesque vehicula dictum augue, eget tristique orci interdum a. Aenean non est eleifend, tristique urna sed, elementum nunc. Sed consectetur id lorem quis mollis.\
Ut et blandit velit, et lobortis dolor. Quisque nec odio sed mi ullamcorper pellentesque. Ut vitae eleifend nisi, vitae dapibus est. Vivamus ornare eleifend volutpat. Sed et tincidunt nisi. Praesent maximus risus a bibendum consequat. Vestibulum quis ex vitae ante ultricies ultricies. Maecenas dictum tellus eget enim tincidunt imperdiet. Quisque vel libero gravida, mollis erat id, placerat dolor.\
Etiam ante eros, bibendum vitae ultricies in, rhoncus nec turpis. Pellentesque gravida nunc sit amet iaculis condimentum. Phasellus in ultricies libero, et maximus justo. Donec ut ultrices elit. Interdum et malesuada fames ac ante ipsum primis in faucibus. Pellentesque rhoncus, nunc at iaculis dictum, magna lectus rhoncus augue, vel aliquam sem mauris in metus. Morbi commodo purus mi, ut faucibus dui luctus et. Suspendisse accumsan placerat tortor. Cras dignissim blandit leo, non tincidunt leo.\
Nulla euismod turpis ac malesuada aliquam. Ut ultrices bibendum elit sed elementum. Donec auctor aliquam vehicula. Mauris lacinia dignissim leo, ullamcorper egestas nibh rutrum eget. In semper sit amet libero eget ultricies. Proin et imperdiet odio. In hac habitasse platea dictumst. In hac habitasse platea dictumst.\
Integer sed dolor quis tortor pretium euismod at vel dolor. Donec aliquet et urna at porta. Vestibulum tincidunt eget sapien elementum mattis. Proin lacinia faucibus orci, sed eleifend augue mollis et. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Cras pellentesque, dolor eget bibendum tincidunt, turpis ante pharetra tortor, quis interdum tellus tellus sit amet nisl. Nulla convallis tempus egestas. Curabitur quis condimentum metus, eu placerat metus. Nunc ligula nunc, posuere at iaculis nec, convallis id tellus. Curabitur pretium libero lorem, quis placerat nunc fringilla interdum.\
Vestibulum et finibus ante. Duis quis nisi neque. Curabitur ornare lorem nec ex fringilla, et porta massa consequat. Nulla malesuada turpis nec eleifend tincidunt. Praesent ultricies dolor vitae mauris lacinia tempor. Sed blandit sapien a odio scelerisque consequat. Mauris non dictum eros. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae;\
Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. In ut sollicitudin tellus. Suspendisse ultrices vitae erat non pharetra. Nulla pellentesque at diam venenatis sollicitudin. Vestibulum sed finibus sapien. Curabitur a metus convallis, euismod est id, iaculis nunc. Vestibulum laoreet ornare turpis. Integer rhoncus, felis nec fermentum suscipit, dui lacus sagittis ligula, vitae vestibulum urna elit aliquam est. Sed sit amet mi tortor. Suspendisse a dapibus velit. Cras eget imperdiet turpis. Maecenas at lorem condimentum, elementum augue mattis, rutrum purus. Duis imperdiet pellentesque nunc, eu tristique lectus malesuada commodo.\
Vivamus aliquet congue eros ac dapibus. Nunc quis porttitor odio. Nulla quis dui luctus, vestibulum enim malesuada, imperdiet elit. Donec facilisis mollis diam ut posuere. Nulla facilisi. Duis nec magna lacus. Vestibulum consequat ut tortor ut ornare. Curabitur nec felis sit amet dui finibus rutrum. Phasellus sit amet lectus nec nisl egestas posuere. Etiam nec euismod magna, vitae ullamcorper enim. Vestibulum pretium cursus semper. Cras vel lorem ut urna molestie elementum. Mauris luctus vel arcu quis egestas. Suspendisse potenti. Nullam viverra sollicitudin lacus luctus sodales.\
Maecenas eget diam cursus quam tincidunt ultricies vitae nec lacus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec urna sapien, porta a efficitur vitae, imperdiet vel ligula. Nulla volutpat massa sit amet est aliquet, ut iaculis tellus convallis. Sed justo tortor, sodales non nisi quis, laoreet commodo quam. Cras tempus purus a tempor malesuada. Curabitur enim nibh, viverra in enim eget, viverra euismod nunc. Mauris nunc leo, faucibus blandit condimentum nec, rutrum sit amet leo. Quisque nec tortor sed felis pretium imperdiet.\
Morbi lobortis, dolor nec lobortis maximus, turpis justo aliquet massa, eget aliquet nunc mauris a lectus. Phasellus dignissim, est nec luctus consequat, ex nisi euismod lacus, a viverra nulla eros et est. Suspendisse in egestas dolor. Etiam non placerat lorem. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Aliquam ut enim tristique, porta nulla quis, placerat eros. Integer eget feugiat mi, ac condimentum felis. Fusce auctor ligula ut est placerat efficitur. Nam hendrerit condimentum ante eget tincidunt. Phasellus vel convallis neque. Vivamus sit amet elit eu enim iaculis scelerisque.\
Donec imperdiet lacus id magna luctus, vitae dapibus quam condimentum. Donec laoreet vehicula tellus. Nullam nec neque at massa ultricies dignissim. Suspendisse potenti. Cras convallis nunc urna, a tempor metus volutpat ut. Fusce viverra lorem vitae quam ullamcorper cursus. Mauris maximus et mi eget tincidunt.\
Proin molestie suscipit felis at ultricies. Duis varius rhoncus metus vehicula bibendum. Aliquam consequat non tellus at aliquam. Vivamus nec turpis facilisis, dapibus lacus in, congue tortor. Curabitur at interdum mi, sed rhoncus nibh. Morbi facilisis purus laoreet, tincidunt justo sit amet, elementum lectus. Vestibulum pellentesque sem lacus, in condimentum purus consequat at.\
Integer pharetra rhoncus aliquam. Duis nec sem ac elit suscipit laoreet. Integer vel est commodo, feugiat sapien eget, cursus quam. Aenean elit leo, interdum a posuere nec, laoreet eu magna. Nam sit amet felis faucibus, porttitor justo eget, commodo mi. Maecenas a eleifend nibh. Donec ut ornare augue. Aliquam pellentesque aliquet eros in hendrerit. Nullam consectetur odio id lectus ullamcorper facilisis.\
Donec pulvinar, magna non sollicitudin commodo, erat lacus egestas massa, a egestas nibh nulla ac lorem. Maecenas at mi posuere, fringilla lectus sed, fringilla eros. Vivamus mattis at magna ac suscipit. Proin varius mi a quam efficitur ullamcorper. Curabitur venenatis turpis lacus, vitae volutpat velit ultricies sed. Sed faucibus id neque in consequat. Nulla imperdiet fermentum placerat. Donec rutrum libero ac lorem commodo pellentesque in tempor augue. Maecenas sodales cursus ex, ac elementum felis consectetur vel.\
Cras ante nulla, porttitor nec ex non, venenatis consectetur justo. Nam vitae enim eget augue euismod suscipit et in nulla. Morbi eu sollicitudin libero, ut lobortis purus. Pellentesque sodales tempor diam, a luctus dui vehicula tempus. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Vestibulum dignissim sagittis diam ac aliquam. Integer iaculis ac est eu molestie. Vivamus convallis arcu nec rutrum molestie. Vestibulum mollis ipsum neque. Vestibulum condimentum neque quis tellus elementum, in facilisis neque venenatis. Donec quis ultrices risus. Cras mattis felis eget erat iaculis, id scelerisque mauris pharetra. Vivamus condimentum tempor ipsum, porta commodo erat dictum ut. Fusce et ligula sed arcu tincidunt efficitur nec ut felis. Donec eu justo pellentesque, finibus diam quis, iaculis erat. Fusce a tempus urna, at fermentum est.\
Sed pretium orci dapibus ante laoreet, a consequat erat scelerisque. Etiam nisi tortor, vulputate quis sapien sit amet, lobortis blandit felis. Morbi urna purus, pellentesque quis orci id, suscipit consequat velit. Donec vehicula ipsum felis. Donec at elit ipsum. Fusce purus sapien, convallis quis faucibus et, tempus at dolor. Vivamus commodo sem ac congue imperdiet. Vivamus convallis eget est eu vulputate. Aliquam vehicula augue ac urna imperdiet interdum. Praesent euismod arcu quis purus vestibulum, et placerat metus hendrerit. Fusce semper lacus sit amet ligula scelerisque scelerisque. Vestibulum neque ex, aliquam non lorem a, aliquam fringilla enim. Aenean consectetur vestibulum tortor. Donec et elit consectetur, tincidunt augue feugiat, condimentum diam. In luctus tellus at massa euismod faucibus.\
Ut tempus dui hendrerit, vehicula ex ut, facilisis lacus. Pellentesque bibendum enim auctor, vulputate justo vel, ultricies est. Praesent interdum turpis in convallis luctus. Duis vel enim venenatis, mollis elit vitae, mattis velit. In eu posuere nibh. Duis a est est. Nam semper tincidunt nulla id dignissim. Fusce consectetur maximus eros quis posuere.\
Sed efficitur, enim quis ultrices eleifend, est diam commodo dui, nec euismod augue velit sit amet ante. Integer fringilla vehicula faucibus. Curabitur non placerat turpis. Integer malesuada quam eget sapien tristique aliquet. In hac habitasse platea dictumst. Cras dignissim mauris neque, in facilisis nulla pulvinar ac. Phasellus sagittis ligula non sem aliquet iaculis. Integer interdum elit in dolor vehicula, non condimentum justo pretium. Aliquam eget feugiat tellus. Suspendisse condimentum dui at erat elementum semper. Aliquam vitae cursus lorem. Ut vestibulum porttitor purus ut dapibus. Curabitur posuere nunc quis nisi rhoncus, ac mollis enim eleifend. Aenean tristique at justo ut tempor.\
Proin posuere condimentum arcu ac lobortis. Proin euismod posuere ipsum, nec dignissim velit eleifend gravida. Quisque quis sem mi. Proin scelerisque consequat lectus nec sodales. Fusce id sapien a erat cursus sodales. Morbi ac magna vitae lorem dictum luctus in et lacus. Morbi imperdiet mi interdum, molestie sem in, accumsan leo. Sed lacinia enim et sem egestas, a pulvinar velit ullamcorper. Aenean laoreet, erat eu viverra dictum, eros odio venenatis mi, tincidunt blandit odio mauris id augue. Donec pretium mauris nibh, ut eleifend velit auctor vitae. Morbi tincidunt lacus id ullamcorper egestas. Proin vel porttitor purus, eu fermentum dui. Aliquam a interdum mi. Aliquam ut rhoncus nibh.\
Morbi nulla libero, commodo quis eros eu, scelerisque gravida ligula. Aliquam sed arcu nunc. Sed egestas hendrerit orci, nec rhoncus arcu fringilla quis. Pellentesque lobortis nulla arcu. Integer aliquam vel quam sed tempor. Morbi viverra tempus risus vel convallis. Cras eget neque ex. Mauris porta, risus at rhoncus hendrerit, libero metus pharetra sapien, quis viverra tortor nunc tincidunt magna. Aenean a tellus ullamcorper, convallis urna quis, suscipit sem. Vivamus eu eleifend est. Duis venenatis metus eget ex consequat molestie. In ullamcorper a dolor vitae feugiat. Morbi ultrices vestibulum venenatis. Phasellus luctus enim id aliquet pharetra. Aenean mauris felis, finibus eu dolor at, tempor sodales diam. Sed nisl nibh, tincidunt quis fringilla vel, congue eu dui.\
Duis viverra justo eu sem ultricies dignissim. Morbi et sollicitudin erat. Proin id porttitor odio, et sagittis ex. Aenean laoreet leo sit amet risus vestibulum, mollis ultrices tortor porttitor. Sed vestibulum varius ligula quis accumsan. Duis fermentum, dolor iaculis condimentum tincidunt, purus nunc bibendum nibh, ac sodales tortor odio non ante. Sed leo mauris, consequat molestie quam eu, vulputate volutpat metus. Cras fringilla risus sed arcu consequat luctus. Nam malesuada, turpis at luctus blandit, velit elit fringilla metus, eu mollis odio felis id tortor. Aliquam erat tellus, pulvinar nec iaculis et, consequat sit amet diam. Sed vestibulum, leo ut vehicula suscipit, quam justo maximus lectus, nec lobortis urna tortor nec nisi. Vestibulum eget ornare arcu, sed viverra turpis. Sed posuere tellus iaculis, scelerisque dui id, convallis lectus.\
Aliquam sodales at mi consectetur dignissim. In fringilla, urna id placerat mattis, diam magna commodo dui, at elementum arcu elit et libero. Duis venenatis vulputate nisl congue pharetra. Fusce sapien velit, cursus a consectetur quis, auctor gravida sem. Maecenas malesuada metus quis elit congue accumsan. Vivamus scelerisque euismod malesuada. Vestibulum purus metus, tempor eget faucibus a, cursus eu arcu. Morbi dictum urna vitae velit pellentesque facilisis. Sed arcu est, tempor ac turpis sit amet, ultricies venenatis augue.\
Nunc laoreet leo gravida facilisis dapibus. Aliquam convallis ullamcorper felis, sit amet tempor libero euismod sit amet. Quisque leo augue, finibus et euismod non, venenatis sed libero. Cras pharetra rhoncus odio, in pharetra lacus porttitor scelerisque. Maecenas eleifend felis vitae diam blandit viverra. Fusce at ultricies arcu, pharetra finibus enim. Etiam pellentesque semper ligula, sed tincidunt purus. Sed fermentum metus varius, aliquet libero eget, vehicula erat. Sed ac finibus metus. Pellentesque libero leo, semper et eros nec, gravida condimentum urna.\
Cras nec turpis convallis, efficitur lacus at, ultricies ex. Fusce eu neque elementum leo gravida semper. Duis sed tellus vitae magna fringilla maximus ac ut nisl. Integer id ligula ullamcorper, ultricies quam sit amet, ullamcorper diam. Maecenas rhoncus nulla eu dui vulputate scelerisque. Vestibulum porttitor eget nibh a mattis. Mauris tempus at urna blandit dignissim. Proin turpis leo, mattis ut turpis eget, aliquet tempor ante. Nunc in mollis nunc, et interdum nisi. Cras tristique sollicitudin tortor sit amet ultrices. Proin rhoncus neque urna.\
Maecenas bibendum, massa sit amet suscipit suscipit, justo tortor maximus dolor, posuere facilisis nisi tellus elementum diam. Quisque id eros vel lectus malesuada tincidunt. Donec at orci ac ligula venenatis dignissim sit amet nec purus. Sed eu neque finibus, tristique ex a, feugiat ante. Pellentesque tincidunt luctus mollis. Nullam blandit faucibus gravida. Ut sit amet malesuada nibh, vel tincidunt ipsum. Donec suscipit lorem in dui luctus, viverra imperdiet magna placerat. Pellentesque venenatis eros quis urna efficitur facilisis.\
Cras ligula magna, tempus facilisis tincidunt at, varius quis lectus. Sed quam neque, facilisis vel facilisis vel, lobortis ac orci. Nullam pretium interdum erat ac ultrices. Etiam enim mauris, vehicula nec rhoncus quis, volutpat vel erat. Morbi imperdiet rhoncus rutrum. Nullam auctor condimentum diam nec faucibus. Etiam sit amet porta nulla, sit amet lobortis enim. Aenean tincidunt condimentum accumsan. Vestibulum mollis diam risus, vitae ornare enim iaculis non. Nullam vitae risus tristique, imperdiet augue ut, egestas dolor. Sed sit amet leo eu diam commodo vestibulum id in dolor. Vivamus tristique molestie faucibus. Duis tempor porttitor turpis ac consectetur.\
Curabitur condimentum, ipsum eu dignissim semper, ipsum erat pretium quam, ut maximus erat ligula eu felis. Sed viverra, mauris id tempus tempor, nisi leo consectetur arcu, ac vulputate lorem mauris non sapien. Maecenas rhoncus magna mauris, in luctus nulla dapibus at. Sed magna est, ultrices sit amet erat nec, dapibus lacinia massa. Morbi cursus ex in elit auctor egestas. Quisque id placerat nibh, at mollis tortor. Proin fringilla sodales sapien, ac ullamcorper sem bibendum eget. Donec dui ligula, viverra eget leo ac, tincidunt fringilla mauris.\
Quisque vel lectus eget metus feugiat laoreet. Morbi eget vestibulum enim, ac ultricies lorem. Nam at mollis magna. Etiam vitae orci eu leo facilisis vestibulum. Ut sed turpis ut nibh iaculis rhoncus. Phasellus sit amet risus pellentesque, gravida eros a, porta nibh. Suspendisse at tincidunt ligula. Vivamus id libero diam. Morbi viverra ipsum turpis, in ullamcorper enim pellentesque nec.\
Sed ultricies, lectus quis pellentesque sodales, arcu diam commodo massa, a vestibulum purus sapien eget risus. Duis rhoncus in velit in dignissim. Aliquam sit amet metus in quam finibus cursus. Pellentesque eget aliquam justo. Fusce imperdiet, tellus non venenatis facilisis, diam mi lobortis dolor, at consectetur est massa id elit. Vivamus ante ex, faucibus et mollis eget, dignissim vel massa. Duis ultricies diam commodo purus facilisis pharetra. Curabitur pretium massa sed enim vehicula, id vehicula neque vehicula.\
In quis lectus non mauris pulvinar fermentum. Aliquam condimentum aliquet dui et congue. Maecenas quis augue eget leo gravida aliquet. Praesent sit amet fermentum odio, ut placerat nulla. Curabitur sit amet iaculis erat, eu volutpat odio. Ut iaculis ex quis tempus commodo. Pellentesque cursus eros at velit vulputate, id luctus massa pretium. Morbi ex dui, sodales id finibus id, aliquet a justo. Maecenas semper leo eget dolor rutrum, at imperdiet nibh eleifend. Aliquam eget purus tortor. Cras rutrum tortor massa, vel bibendum nunc aliquam vel. Nullam vestibulum, metus vel fermentum elementum, nulla sapien egestas justo, ac feugiat ex justo nec eros.\
Donec sit amet nibh mollis, commodo quam sit amet, semper magna. In tortor magna, elementum nec auctor sed, pellentesque at augue. Sed gravida arcu ac aliquet convallis. Nulla facilisi. Duis nunc quam, gravida non interdum id, cursus ac leo. Suspendisse vel ipsum nisl. Aliquam at gravida libero. Maecenas sit amet efficitur orci. Fusce id vehicula sapien. Proin euismod diam non laoreet ultricies. Nunc ullamcorper, nibh id cursus vehicula, ex purus tempor urna, et euismod orci est sed elit. Duis ut blandit mauris.\
Ut blandit cursus eros, sed laoreet nisl efficitur ac. Phasellus dui elit, fringilla sit amet cursus nec, pharetra quis odio. Ut ut lorem sit amet sem dapibus accumsan. Aenean a laoreet dolor. Donec eu laoreet velit. Etiam id nisi vel nibh dapibus congue a quis odio. Donec velit risus, semper quis porta non, elementum quis lorem. Interdum et malesuada fames ac ante ipsum primis in faucibus. Nullam sit amet dolor magna. Maecenas quis sapien sit amet est pulvinar lobortis efficitur cursus orci. Phasellus tristique mauris lorem, eu ultricies justo ornare condimentum.\
Integer urna enim, lobortis id malesuada ut, mattis eget libero. Sed commodo tincidunt eleifend. Fusce sed velit ut dui pellentesque pellentesque eget vel diam. Aenean nec turpis at tortor consectetur consectetur. Vestibulum ultrices elit at nisl pellentesque molestie. Maecenas diam dolor, faucibus eget posuere ut, sodales ut eros. Nam vulputate mollis diam nec gravida. Nam et ullamcorper diam. Aenean non nulla non lorem ullamcorper sagittis non quis erat. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. In hac habitasse platea dictumst.\
Donec quis mauris ac nibh vestibulum eleifend placerat sed lacus. Suspendisse mi elit, viverra non velit ut, tincidunt tempus felis. Fusce ullamcorper, arcu nec aliquet porttitor, odio lacus mollis mi, id malesuada tortor velit aliquet turpis. Sed hendrerit felis nec faucibus ornare. Nulla ut metus eget augue malesuada posuere eget eu tortor. Cras ultrices odio sit amet porttitor vehicula. Sed vulputate leo vitae justo viverra, nec volutpat eros consectetur. Nunc nunc tellus, porta in arcu in, vulputate ultricies tellus. Fusce commodo efficitur lorem, sit amet lacinia sapien sollicitudin at. Etiam aliquet non mi vitae ornare. Cras condimentum imperdiet elit eu dictum. Donec sed enim sed massa tempor porta et sit amet felis.\
Nam interdum ornare sem, in tincidunt risus consectetur vel. Ut convallis purus mauris, in consequat ligula ullamcorper ut. Quisque elit ipsum, accumsan eget ligula vitae, sollicitudin luctus tellus. Nunc pretium turpis ligula, id dignissim lorem suscipit eu. Nulla facilisi. Sed lectus odio, vehicula vel vulputate id, ultrices non ipsum. Donec arcu quam, consequat eget aliquet sit amet, ullamcorper non nibh. Etiam finibus, mi id lobortis sagittis, leo leo lobortis lectus, sit amet aliquam dui odio sit amet massa. Suspendisse iaculis urna ac lectus gravida, iaculis efficitur tellus hendrerit. Sed tellus enim, condimentum in augue eget, sagittis ullamcorper sem. Suspendisse vitae aliquet libero. Aenean quis purus in sapien dapibus suscipit. Sed commodo nunc in lacus bibendum, vel tincidunt ante ornare.\
Ut tristique luctus volutpat. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Quisque a ultricies orci, eu porta odio. Vivamus sapien arcu, ultrices vel dui ut, luctus viverra purus. Praesent fringilla sed odio quis pretium. Vestibulum ullamcorper nisi tortor, id sollicitudin lectus tempor a. Ut malesuada sapien eu sapien posuere, non euismod eros porta. Nunc vel tincidunt ligula. Cras dolor ante, tristique tempor metus quis, mollis vulputate orci. Curabitur vitae nisl euismod, elementum purus vel, dictum lorem. Nunc eu mauris at metus porttitor dignissim ut eu neque.\
In tempor rhoncus neque sit amet commodo. Maecenas sed lacus semper, tempus enim ac, fermentum lorem. Nullam sollicitudin convallis turpis. Curabitur finibus placerat viverra. Pellentesque convallis condimentum tortor id efficitur. Proin semper pretium est, et vehicula ex cursus a. Nam ut felis purus. Phasellus eget felis eget leo dapibus vestibulum. Nulla eleifend malesuada turpis, quis faucibus eros. Nam aliquet euismod viverra. Ut quis semper felis. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Pellentesque at nulla arcu. Integer ut tellus ac sapien maximus tincidunt sed vitae risus.\
Nulla viverra, nibh eget eleifend aliquam, quam quam tempor massa, eu semper ipsum lacus in turpis. Nulla sed purus enim. Nullam sed fermentum ipsum. Sed dui nisi, elementum a auctor at, ultrices et nibh. Phasellus aliquam nulla ut lacinia accumsan. Phasellus sed arcu ligula. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam fermentum magna vitae dui sagittis tempor.\
Vivamus eu ligula blandit, imperdiet arcu at, rutrum sem. Aliquam erat volutpat. Quisque luctus enim quis volutpat lobortis. Vestibulum eget sodales libero. Aenean at condimentum est. Proin eget massa vel nulla efficitur tempor eget at enim. Integer enim sapien, ornare luctus nisl non, pretium facilisis ex. Donec pretium ligula ligula, a facilisis turpis hendrerit at.\
Nullam eget malesuada justo, at molestie quam. Sed consequat massa eu faucibus maximus. Curabitur placerat orci sapien, sit amet semper magna sodales non. Ut fermentum accumsan odio in consectetur. Morbi neque mi, vulputate nec mi ut, cursus scelerisque lectus. Nulla sapien enim, finibus id ipsum luctus, consequat ullamcorper lectus. Sed volutpat sed massa in sodales. Morbi lacinia diam eu commodo vulputate. Fusce aliquet pulvinar dolor in egestas. Fusce molestie commodo leo eu ultricies. Nulla mollis rhoncus pharetra. Pellentesque rutrum mauris ac lorem posuere, a eleifend mi rutrum. Nulla porta turpis aliquet felis congue rutrum. Fusce quis arcu in sem placerat condimentum a ut turpis. Quisque quis porttitor nulla. Donec sit amet quam tincidunt, pulvinar erat id, molestie dolor.\
Praesent luctus vitae nunc vitae pellentesque. Praesent faucibus sed urna ut lacinia. Vivamus id justo quis dolor porta rutrum nec nec odio. Cras euismod tortor quis diam ultrices, eu mattis nisi consectetur. Fusce mattis nisi vel condimentum molestie. Fusce fringilla ut nibh volutpat elementum. Mauris posuere consectetur leo a aliquet. Donec quis sodales sapien. Maecenas ut felis tempus, eleifend mauris et, faucibus mi. Quisque fringilla orci arcu, sit amet porta risus hendrerit non. Aenean id sem nisi. Nullam non nisl vestibulum, pellentesque nisl et, imperdiet ligula. Sed laoreet fringilla felis. Proin ac dolor viverra tellus mollis aliquet eget et neque. Suspendisse mattis nulla vitae nulla sagittis blandit. Sed at tortor rutrum, ornare magna nec, pellentesque nisi. Etiam non aliquet tellus. Aliquam at ex suscipit, posuere sem sit amet, tincidunt.";

/* Using http://corpora.uni-leipzig.de/downloads/eng_wikipedia_2010_1M-text.tar.gz (1 million of words, about 122MB of text)
 * from http://corpora.informatik.uni-leipzig.de/download.html */
#define TEXT_CORPUS_PATH "/home/i74700deb64/Téléchargements/eng_wikipedia_2010_1M-text/eng_wikipedia_2010_1M-sentences.txt"

#define PRINTF_GHASH_STATS(_gh) \
{ \
	double q, lf; \
	int minb, maxb; \
	q = BLI_ghash_calc_quality((_gh), &lf, &minb, &maxb); \
	printf("GHash stats:\n\tQuality (the lower the better): %f\n\tLoad: %f\n\tSmallest bucket: %d\n\tBiggest bucket: %d\n", \
	       q, lf, minb, maxb); \
} void (0)


/* Str: whole text, lines and words from a 'corpus' text. */

static void str_ghash_tests(GHash *ghash, const char *id)
{
	printf("\n========== SARTING %s ==========\n", id);

#ifdef TEXT_CORPUS_PATH
	size_t sz = 0;
	char *data;
	{
		struct stat st;
		if (stat(TEXT_CORPUS_PATH, &st) == 0)
			sz = st.st_size;
	}
	if (sz != 0) {
		FILE *f = fopen(TEXT_CORPUS_PATH, "r");

		data = (char *)MEM_mallocN(sz + 1, __func__);
		if (fread(data, sizeof(*data), sz, f) != sz) {
			printf("ERROR in reading file %s!", TEXT_CORPUS_PATH);
			MEM_freeN(data);
			data = BLI_strdup(words10k);
		}
		data[sz] = '\0';
		fclose(f);
	}
	else {
		data = BLI_strdup(words10k);
	}
#else
	char *data = BLI_strdup(words10k);
#endif
	char *data_p = BLI_strdup(data);
	char *data_w = BLI_strdup(data);
	char *data_bis = BLI_strdup(data);

	{
		char *p, *w, *c_p, *c_w;

		TIMEIT_START(string_insert);

		BLI_ghash_insert(ghash, data, SET_INT_IN_POINTER(data[0]));

		for (p = c_p = data_p, w = c_w = data_w; *c_w; c_w++, c_p++) {
			if (*c_p == '\n') {
				*c_p = *c_w = '\0';
				BLI_ghash_add(ghash, p, SET_INT_IN_POINTER(p[0]));
				BLI_ghash_add(ghash, w, SET_INT_IN_POINTER(w[0]));
				p = c_p + 1;
				w = c_w + 1;
			}
			else if (*c_w == ' ') {
				*c_w = '\0';
				BLI_ghash_add(ghash, w, SET_INT_IN_POINTER(w[0]));
				w = c_w + 1;
			}
		}

		TIMEIT_END(string_insert);
	}

	PRINTF_GHASH_STATS(ghash);

	{
		char *p, *w, *c;
		void *v;

		TIMEIT_START(string_lookup);

		v = BLI_ghash_lookup(ghash, data_bis);
		EXPECT_EQ(data_bis[0], GET_INT_FROM_POINTER(v));

		for (p = w = c = data_bis; *c; c++) {
			if (*c == '\n') {
				*c = '\0';
				v = BLI_ghash_lookup(ghash, w);
				EXPECT_EQ(w[0], GET_INT_FROM_POINTER(v));
				v = BLI_ghash_lookup(ghash, p);
				EXPECT_EQ(p[0], GET_INT_FROM_POINTER(v));
				p = w = c + 1;
			}
			else if (*c == ' ') {
				*c = '\0';
				v = BLI_ghash_lookup(ghash, w);
				EXPECT_EQ(w[0], GET_INT_FROM_POINTER(v));
				w = c + 1;
			}
		}

		TIMEIT_END(string_lookup);
	}

	BLI_ghash_free(ghash, NULL, NULL);
	MEM_freeN(data);
	MEM_freeN(data_p);
	MEM_freeN(data_w);
	MEM_freeN(data_bis);

	printf("========== ENDED %s ==========\n\n", id);
}

TEST(ghash, TextGHash)
{
	GHash *ghash = BLI_ghash_new(BLI_ghashutil_strhash_p, BLI_ghashutil_strcmp, __func__);

	str_ghash_tests(ghash, "StrGHash - GHash");
}

TEST(ghash, TextMurmur2a)
{
	GHash *ghash = BLI_ghash_new(BLI_ghashutil_strhash_p_murmur, BLI_ghashutil_strcmp, __func__);

	str_ghash_tests(ghash, "StrGHash - Murmur");
}


/* Int: uniform 50M first integers. */

static void int_ghash_tests(GHash *ghash, const char *id)
{
	printf("\n========== SARTING %s ==========\n", id);

	const unsigned int nbr = 50000000;
	{
		unsigned int i = nbr;

		TIMEIT_START(int_insert);

		while (i--) {
			BLI_ghash_insert(ghash, SET_UINT_IN_POINTER(i), SET_UINT_IN_POINTER(i));
		}

		TIMEIT_END(int_insert);
	}

	PRINTF_GHASH_STATS(ghash);

	{
		unsigned int i = nbr;

		TIMEIT_START(int_lookup);

		while (i--) {
			void *v = BLI_ghash_lookup(ghash, SET_UINT_IN_POINTER(i));
			EXPECT_EQ(i, GET_UINT_FROM_POINTER(v));
		}

		TIMEIT_END(int_lookup);
	}

	BLI_ghash_free(ghash, NULL, NULL);

	printf("========== ENDED %s ==========\n\n", id);
}

TEST(ghash, IntGHash)
{
	GHash *ghash = BLI_ghash_new(BLI_ghashutil_inthash_p, BLI_ghashutil_intcmp, __func__);

	int_ghash_tests(ghash, "IntGHash - GHash");
}

TEST(ghash, IntMurmur2a)
{
	GHash *ghash = BLI_ghash_new(BLI_ghashutil_inthash_p_murmur, BLI_ghashutil_intcmp, __func__);

	int_ghash_tests(ghash, "IntGHash - Murmur");
}


/* Int_v4: 10M of randomly-generated integer vectors. */

static void int4_ghash_tests(GHash *ghash, const char *id)
{
	printf("\n========== SARTING %s ==========\n", id);

	const unsigned int nbr = 10000000;
	unsigned int (*data)[4] = (unsigned int (*)[4])MEM_mallocN(sizeof(*data) * (size_t)nbr, __func__);
	unsigned int (*dt)[4];
	unsigned int i, j;

	{
		RNG *rng = BLI_rng_new(0);
		for (i = nbr, dt = data; i--; dt++) {
			for (j = 4; j--; ) {
				(*dt)[j] = BLI_rng_get_uint(rng);
			}
		}
		BLI_rng_free(rng);
	}

	{
		TIMEIT_START(int_v4_insert);

		for (i = nbr, dt = data; i--; dt++) {
			BLI_ghash_insert(ghash, *dt, SET_UINT_IN_POINTER(i));
		}

		TIMEIT_END(int_v4_insert);
	}

	PRINTF_GHASH_STATS(ghash);

	{
		TIMEIT_START(int_v4_lookup);

		for (i = nbr, dt = data; i--; dt++) {
			void *v = BLI_ghash_lookup(ghash, (void *)(*dt));
			EXPECT_EQ(i, GET_UINT_FROM_POINTER(v));
		}

		TIMEIT_END(int_v4_lookup);
	}

	BLI_ghash_free(ghash, NULL, NULL);
	MEM_freeN(data);

	printf("========== ENDED %s ==========\n\n", id);
}

TEST(ghash, Int4GHash)
{
	GHash *ghash = BLI_ghash_new(BLI_ghashutil_uinthash_v4_p, BLI_ghashutil_uinthash_v4_cmp, __func__);

	int4_ghash_tests(ghash, "Int4GHash - GHash");
}

TEST(ghash, Int4Murmur2a)
{
	GHash *ghash = BLI_ghash_new(BLI_ghashutil_uinthash_v4_p_murmur, BLI_ghashutil_uinthash_v4_cmp, __func__);

	int4_ghash_tests(ghash, "Int4GHash - Murmur");
}
