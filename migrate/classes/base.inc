<?php

/**
 * @file
 * Basic class for common properties.
 */

/**
 * Class d7ExampleBase
 */
abstract class d7ExampleBase extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    // Source and destination relation for rollbacks
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'title' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Topic ID',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Set defaults for unmapped fields
    $this->addFieldMapping('promote')->defaultValue(0);
    $this->addFieldMapping('status')->defaultValue(1);
    $this->addFieldMapping('language')->defaultValue(LANGUAGE_NONE);
  }
}
