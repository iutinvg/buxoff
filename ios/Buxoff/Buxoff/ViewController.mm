//
//  ViewController.m
//  Buxoff
//
//  Created by Viacheslav Iutin on 30/07/2016.
//  Copyright © 2016 Viacheslav Iutin. All rights reserved.
//

#import "ViewController.h"
#import "StorageWrapper.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    _sw = [[StorageWrapper alloc] init];    
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self setupCounter];
}

#pragma - Mark Layout Helpers
- (void)setupCounter
{
    _labelCounter.text = [NSString stringWithFormat:@"total records: %ld", [_sw getCount]];
}

#pragma mark - Actions
- (void)actionAdd:(id)sender
{
    NSString* uid = [_sw addTestRecord];
    NSLog(@"new record was added %@ ...", uid);
    [self setupCounter];
}

@end
