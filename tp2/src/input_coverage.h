/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************
 
  stimulus.h -- 
 
  Original Author: Rocco Jonack, Synopsys, Inc.
 
 *****************************************************************************/

 /*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here. 
      Name, Affiliation: Teodor Vasilache and Dragos Dospinescu,
                         AMIQ Consulting s.r.l. (contributors@amiq.com)
                   Date: 2018-Feb-20

  Description of Modification: Included the FC4SC library, created and 
  instantiated a covergroup for collecting stimulus data coverage.

  	  	  	  	   Date: 2018-Sep-24

  Description of Modification: Updated the instantiation of the coverpoints
  to use the new COVERPOINT macro.
            
 *****************************************************************************/

/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#include "fc4sc.hpp"
#include "my_random_obj.h" //include pour les enums


  class input_coverage : public covergroup {

  public:

	unsigned copro;
	unsigned sort_dir;
	unsigned data_order;
	unsigned int address;
    int nb_de_cov = 0;

    // Must call parent constructor somewhere register a new cvg
    CG_CONS(input_coverage) {
    }

    void  sample(unsigned copro, unsigned sort_dir , unsigned data_order/*, unsigned int address*/) {

      this->copro = copro;
      this->sort_dir = sort_dir;
      this->data_order = data_order;/*
	  this->address = address;*/
      covergroup::sample();
      nb_de_cov += 1;

    }

    COVERPOINT(unsigned, copro_cvp, copro) {
        bin<unsigned>("copro 1", copro1),
        bin<unsigned>("copro 2", copro2),
        bin<unsigned>("copro 3", copro3) //à vérifier si virugle ou pas sur la dernière ligne
    };

    COVERPOINT(unsigned, sort_dir_cvp, sort_dir) {
        bin<unsigned>("ascendant", up),
        bin<unsigned>("descendant",down)
    };

    COVERPOINT(unsigned, data_order_cvp, data_order) {
		bin<unsigned>("random_desc", random_desc),
		bin<unsigned>("random_asc", random_asc),
		bin<unsigned>("random_full", random_full),
        bin<unsigned>("continues_asc", continues_asc),
        bin<unsigned>("continues_desc", continues_desc)
    };

	/*
	COVERPOINT(unsigned int, adress_cvp, address) {
		//la je n'ai pas trop d'idées poour les bins des adresses
		//il faudrait idéalement toutes les mettre mais il y a beaucoup de possibilités, peut-on générer un bin avec une boucle for ?
		// tester les dernières adresses
		bin<unsigned int>("1 ere adresse possible copro1", 0),
		bin<unsigned int>("2 eme adresse possible copro1", 256),
		bin<unsigned int>("3 eme adresse possible copro1", 512)
	};
	*/

    cross<unsigned, unsigned, unsigned/*, unsigned int*/> reset_valid_cross = cross<unsigned, unsigned, unsigned/*, unsigned int*/> (this, "croissement de 3(?4) parametres",
        &data_order_cvp,
        &sort_dir_cvp,
		    &copro_cvp/*,
        &address_cvp*/
    );

  };


